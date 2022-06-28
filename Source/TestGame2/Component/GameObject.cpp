// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObject.h"
#include "../ETC/SDB.h"
#include "../ETC/CameraShakeEffect.h"
#include "../Manager/ObjectManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGameObject::UGameObject()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;
	IsAttackMove = false;
	IsEnabledAttackColl = false;
	AnimState = EAnimState::IDLE_RUN;
}

UGameObject::~UGameObject()
{
	CoolingSkills.Reset();
	SkillInfos.Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto hitColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "HitColl" ) ) ) : nullptr;
	if( hitColl )
		hitColl->OnComponentBeginOverlap.AddDynamic( this, &UGameObject::HitCollBeginOverlap );

	SetMoveSpeed( Stat.MoveSpeed );
	SetJumpPower( Stat.JumpPower );

	for( const auto& skill : SkillInfos )
		CoolingSkills.Add( skill.Num, 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ResetInfo();

	_AnimStateChange();
	_CheckDie();
	_Move();
	_CoolingSkills( DeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 정보를 초기화한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::ResetInfo( bool InForceReset )
{
	if( InForceReset ||
		AnimState == EAnimState::IDLE_RUN ||
		AnimState == EAnimState::JUMP ||
		AnimState == EAnimState::DIE )
	{
		SetIsEnableDerivedKey( false );
		SetIsAttackMove( false );
		SetIsEnabledAttackColl( false );
		SetAttackCollInfo( FCollisionInfo() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 대상을 바라본다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::LookAt( ACharacter* InTarget )
{
	if( !OwningCharacter || !InTarget )
		return;

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( OwningCharacter->GetActorLocation(), InTarget->GetActorLocation() );
	rotator.Pitch = OwningCharacter->GetActorRotation().Pitch;
	OwningCharacter->SetActorRotation( rotator );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 몽타주를 플레이한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::MontagePlay( UAnimMontage* InMontage, float InScale )
{
	if( !OwningCharacter )
		return;

	UAnimInstance* animInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if( !animInstance || !InMontage )
		return;

	ResetInfo( true );
	animInstance->Montage_Play( InMontage, InScale );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬을 플레이한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UGameObject::SkillPlay( int InSkillNum, float InScale )
{
	for( auto& skillInfo : SkillInfos )
	{
		if( InSkillNum != skillInfo.Num )
			continue;

		if( IsCoolingSkill( skillInfo.Num ) )
			continue;
			
		MontagePlay( skillInfo.Anim, InScale );
		_RegisterCoolTime( skillInfo );

		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 카메라 쉐이크를 실행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::CameraShake( float InScale, bool InShakeByWeight )
{
	if( InShakeByWeight )
	{
		auto moveComponent = OwningCharacter->GetCharacterMovement();
		if( moveComponent )
		{
			if( !moveComponent->IsFalling() && Stat.JumpPower <= 0.5f )
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), InScale );
			}
		}
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), InScale );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 콜리전 정보를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo )
{
	AttackCollInfo = InAttackCollInfo;

	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
	{ 
		attackColl->SetBoxExtent( AttackCollInfo.Size );
		attackColl->SetRelativeLocation( AttackCollInfo.Pos );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동속도를 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetMoveSpeed( float InMoveSpeed )
{
	Stat.MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = Stat.MoveSpeed * Const::DEFAULT_MOVE_SPEED;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 점프력을 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetJumpPower( float InJumpPower )
{
	Stat.JumpPower = InJumpPower;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->JumpZVelocity = Stat.JumpPower * Const::DEFAULT_JUMP_POWER;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 콜리전 활성화 여부를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetIsEnabledAttackColl( bool InIsEnabledAttackColl )
{
	IsEnabledAttackColl = InIsEnabledAttackColl;

	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
	{ 
		attackColl->SetCollisionEnabled( IsEnabledAttackColl ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision );
		attackColl->SetVisibility( IsEnabledAttackColl );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동할 위치를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetMovePos( float InMovePower )
{
	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

	const FRotator Rotation = characterMovement->GetLastUpdateRotation();
	const FRotator YawRotation( 0, Rotation.Yaw, 0 );
	const FVector  Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

	MovePos = characterMovement->GetActorLocation() + ( Direction * ( InMovePower * Stat.MoveSpeed ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 현재 몽타주 이름을 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
FString UGameObject::GetCurMontageName()
{
	auto curMontage = OwningCharacter ? OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() : nullptr;
	FString curMontageName = curMontage ? curMontage->GetName() : "";

	return curMontageName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 해당 스킬이 쿨타임이 돌고 있는지 여부를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UGameObject::IsCoolingSkill( int InSkillNum )
{
	if( CoolingSkills.FindRef( InSkillNum ) > 0 )
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OwningCharacter ) 
		return;

	// 자기 자신 충돌은 무시한다.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter ) 
		return;

	// AttackColl이 충돌된 것이 아니면 무시한다.
	auto boxComponent = Cast<UBoxComponent>( OtherComp );
	if( !boxComponent || !boxComponent->GetName().Equals( TEXT( "AttackColl" ) ) )
		return;
	
	// 체력 감소
	auto othetGameObject = OtherActor ? Cast<UGameObject>( OtherActor->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( othetGameObject )
	{ 
		float decrease = Stat.Hp - othetGameObject->GetAttackCollInfo().Power;
		SetHp( decrease > 0 ? decrease : 0 );
	}

	FString str = OwningCharacter->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Stat.Hp );
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

	CameraShake();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 애니메이션 상태를 교체한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_AnimStateChange()
{
	if( Stat.Hp <= 0 )
	{
		AnimState = EAnimState::DIE;
		return;
	}

	if( !OwningCharacter )
		return;

	if( !OwningCharacter->GetMesh()->GetAnimInstance() )
		return;

	auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	if( curMontage )
	{
		AnimState = EAnimState::COMMON_ACTION;
	}
	else
	{
		auto moveComponent = OwningCharacter->GetMovementComponent();
		if( moveComponent )
			moveComponent->IsFalling() ? AnimState = EAnimState::JUMP : AnimState = EAnimState::IDLE_RUN;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동 관련 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_Move()
{
	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

	if( !characterMovement )
		return;

	if( AnimState == EAnimState::IDLE_RUN || AnimState == EAnimState::JUMP )
	{
		characterMovement->MaxWalkSpeed = Stat.MoveSpeed * Const::DEFAULT_MOVE_SPEED;
	}
	else if( AnimState == EAnimState::DIE )
	{
		characterMovement->MaxWalkSpeed = 0;
	}
	else
	{
		if( IsAttackMove )
		{
			float dest_X = FMath::Lerp( characterMovement->GetActorLocation().X, MovePos.X, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			float dest_Y = FMath::Lerp( characterMovement->GetActorLocation().Y, MovePos.Y, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			float dest_Z = FMath::Lerp( characterMovement->GetActorLocation().Z, MovePos.Z, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			FVector dest = FVector( dest_X, dest_Y, dest_Z );

			OwningCharacter->SetActorLocation( dest, true );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 해당 캐릭터가 사망했는지 체크한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_CheckDie()
{
	if( !OwningCharacter )
		return;

	if( AnimState == EAnimState::DIE )
	{
		GetObjectManager().DestroyActor( OwningCharacter );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬 쿨타임을 등록한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_RegisterCoolTime( const FSkillInfo& InSkillInfo )
{
	if( InSkillInfo.CoolTime <= 0 )
		return;

	if( float* coolTime = CoolingSkills.Find( InSkillInfo.Num ) )
		*coolTime = InSkillInfo.CoolTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬 쿨타임을 돌린다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_CoolingSkills( float InDeltaTime )
{
	for( auto& pair : CoolingSkills )
	{
		float& remainCoolTime = pair.Value;

		if( remainCoolTime > 0 )
			remainCoolTime -= InDeltaTime;
		else
			continue;

		if( remainCoolTime < 0 )
			remainCoolTime = 0;
	}
}