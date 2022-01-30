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
//// @brief ������ �ʱ�ȭ�Ѵ�.
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
//// @brief ��Ÿ�ָ� �÷����Ѵ�.
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
//// @brief ���� �ݸ��� ������ �����Ѵ�.
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
//// @brief �̵��ӵ��� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::SetMoveSpeed( float InMoveSpeed )
{
	MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = MoveSpeed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
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
//// @brief �̵� �ӵ��� ����ġ�� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::MultiplyMoveSpeed( float InMoveMultipler )
{
	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed *= InMoveMultipler;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
FString UGameObject::GetCurMontageName()
{
	auto curMontage = OwningCharacter ? OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() : nullptr;
	FString curMontageName = curMontage ? curMontage->GetName() : "";

	return curMontageName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UGameObject::HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OwningCharacter ) 
		return;

	// �ڱ� �ڽ� �浹�� �����Ѵ�.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter ) 
		return;

	// AttackColl�� �浹�� ���� �ƴϸ� �����Ѵ�.
	auto boxComponent = Cast<UBoxComponent>( OtherComp );
	if( !boxComponent || !boxComponent->GetName().Equals( TEXT( "AttackColl" ) ) )
		return;
	
	// ü�� ����
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
//// @brief �ִϸ��̼� ���¸� ��ü�Ѵ�.
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
//// @brief �̵� ���� ������ �����Ѵ�.
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
//// @brief �ش� ĳ���Ͱ� ����ߴ��� üũ�Ѵ�.
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