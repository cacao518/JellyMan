// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObject.h"
#include "../ETC/SDB.h"
#include "../ETC/CameraShakeEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

UGameObject::UGameObject()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;
	IsAttackMove = false;
	IsEnabledAttackColl = false;
	AnimState = EAnimState::IDLE_RUN;
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
		SetIsAttackMove( false );
		SetIsEnabledAttackColl( false );
	}
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

	animInstance->Montage_Play( InMontage, InScale );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 콜리전 정보를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetAttackCollInfo( const CollisionInfo& InAttackCollInfo ) 
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
	MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = MoveSpeed;
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
//// @brief 이동 속도에 가중치를 곱한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::MultiplyMoveSpeed( float InMoveMultipler )
{
	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed *= InMoveMultipler;
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
		float decrease = Hp - othetGameObject->GetAttackCollInfo().Power;
		SetHp( decrease > 0 ? decrease : 0 );
	}

	FString str = OwningCharacter->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Hp );
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), 1.f );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 애니메이션 상태를 교체한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::_AnimStateChange()
{
	if( Hp <= 0 )
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
		characterMovement->MaxWalkSpeed = MoveSpeed;
	}
	else if( AnimState == EAnimState::DIE )
	{
		characterMovement->MaxWalkSpeed = 0;
	}
	else
	{
		if( IsAttackMove )
		{
			const FRotator Rotation = characterMovement->GetLastUpdateRotation();
			const FRotator YawRotation( 0, Rotation.Yaw, 0 );
			const FVector  Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

			characterMovement->MaxWalkSpeed = FMath::Lerp( characterMovement->MaxWalkSpeed, MoveSpeed, GetWorld()->GetDeltaSeconds()*Const::ANIM_LERP_MULITPLIER );
			OwningCharacter->AddMovementInput( Direction, 1.0f );
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
		OwningCharacter->Destroy();
	}
}