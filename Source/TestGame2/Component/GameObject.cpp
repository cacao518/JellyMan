// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObject.h"
#include "../ETC/SDB.h"
#include "../ETC/CameraShakeEffect.h"
#include "../Character/GamePlayer.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
#include "Kismet/KismetMathLibrary.h"

UGameObject::UGameObject()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;
	IsForceMove = false;
	IsEnabledAttackColl = false;
	IsDie = false;
	IsFallWater = false;
	FallWaterTimeAmount = 0;
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

	_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_AnimStateChange();
	_CheckDie();
	_Move();
	_CoolingSkills( DeltaTime );
	_FallingWater( DeltaTime );

	ResetInfo();
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
		SetIsEnabledAttackColl( false );
		SetAttackCollInfo( FCollisionInfo() );
		IsEnableDerivedKey = false;
		IsForceMove = false;
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
void UGameObject::SetMovePos( float InMovePower, bool InIsKnockBack )
{
	if( !OwningCharacter )
		return;

	const FRotator rotation = OwningCharacter->GetActorRotation();
	const FRotator yawRotation( 0, rotation.Yaw, 0 );
	const FVector  direction = FRotationMatrix( yawRotation ).GetUnitAxis( EAxis::X );

	if( InIsKnockBack )
		MovePos = OwningCharacter->GetActorLocation() - ( direction * InMovePower );
	else
		MovePos = OwningCharacter->GetActorLocation() + ( direction * ( InMovePower * Stat.MoveSpeed ) );
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

	auto boxComponent = Cast<UBoxComponent>( OtherComp );
	if( boxComponent && boxComponent->GetName().Equals( TEXT( "AttackColl" ) ) )
	{
		_ProcessHit( OtherActor );
		return;
	}

	_ProcessLandscapeHit( OtherActor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 초기화 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_Init()
{
	InitStat = Stat;

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
		if( IsForceMove )
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
	else
		CoolingSkills.Add( InSkillInfo.Num, InSkillInfo.CoolTime );
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 물에 빠지는 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_FallingWater( float InDeltaTime )
{
	if( !IsFallWater || Stat.Hp <= 0 )
		return;

	if( FallWaterTimeAmount <= 3.f )
	{
		FallWaterTimeAmount += InDeltaTime;
		FVector originPos = OwningCharacter->GetMesh()->GetRelativeLocation();
		FVector resultPos = FVector( originPos.X, originPos.Y, originPos.Z - 1.3f );
		OwningCharacter->GetMesh()->SetRelativeLocation( resultPos );
	}
	else
	{
		Stat.Hp = 0;
		IsFallWater = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 피격 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_ProcessHit( AActor* InOtherActor )
{
	auto othetGameObject = InOtherActor ? Cast<UGameObject>( InOtherActor->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !othetGameObject )
		return;

	// 체력 감소
	float totalDamage = othetGameObject->GetAttackCollInfo().Power * othetGameObject->GetStat().AttackPower;
	totalDamage -= Stat.DefensePower;
	totalDamage = totalDamage > 0 ? totalDamage : 1;

	float decrease = Stat.Hp - totalDamage;
	Stat.Hp = decrease > 0 ? decrease : 0;

	// 경직
	if( Stat.Hpm * ( 0.1f + ( Stat.Strength * 0.005f ) ) < totalDamage )
	{
		MontagePlay( HitAnim, 1.0f + ( Stat.Strength * 0.01f ) );
		LookAt( Cast<ACharacter>( InOtherActor ) );

		// 넉백
		float knockbackPower = othetGameObject->GetAttackCollInfo().KnockBackPower - ( Stat.Strength * 1.5f );
		if( knockbackPower > 0 )
		{
			SetMovePos( knockbackPower, true );
			SetIsForceMove( true );
		}
	}

	FString str = OwningCharacter->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Stat.Hp );
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

	CameraShake();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 랜드스케이프 피격 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_ProcessLandscapeHit( AActor* InOtherActor )
{
	// 랜드스케이프 물에 빠졌을 때
	auto landScape = Cast<ULandscapeComponent>( InOtherActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
	if( !landScape )
		return;

	auto proxy = landScape->GetLandscapeProxy();
	if( !proxy )
		return;

	UMaterialInterface* matInterface = proxy->GetLandscapeMaterial( 0 );
	if( !matInterface )
		return;

	const MaterialInfoList& matInfoList = GetDataInfoManager().GetMaterialInfos();
	auto iter = matInfoList.find( EMaterialState::WATER );
	if( iter == matInfoList.end() )
		return;

	if( ( *iter ).second.AssetPath == matInterface->GetPathName() )
	{
		MontagePlay( HitAnim, 0.3f );
		IsFallWater = true;
	}
}
