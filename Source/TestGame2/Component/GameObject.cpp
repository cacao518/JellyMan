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

void UGameObject::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto hitColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "HitColl" ) ) ) : nullptr;
	if( hitColl )
		hitColl->OnComponentBeginOverlap.AddDynamic( this, &UGameObject::HitCollBeginOverlap );
}

void UGameObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_AnimStateChange();
	_Move();
	_ResetInfo();
}

void UGameObject::SetAttackCollInfo()
{
	AttackCollInfo = CollisionInfo();

	SetIsAttackMove( false );
	SetIsEnabledAttackColl( false );

	SetAttackCollSize( AttackCollInfo.Size );
	SetAttackCollPos( AttackCollInfo.Pos );
	SetAttackCollPower( AttackCollInfo.Power );
}

void UGameObject::SetAttackCollInfo( const CollisionInfo& InAttackCollInfo ) 
{
	AttackCollInfo = InAttackCollInfo;

	SetIsAttackMove( false );
	SetIsEnabledAttackColl( false );

	SetAttackCollSize( AttackCollInfo.Size );
	SetAttackCollPos( AttackCollInfo.Pos );
	SetAttackCollPower( AttackCollInfo.Power );
}

void UGameObject::SetMoveSpeed( float InMoveSpeed )
{
	MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = MoveSpeed;
}

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

void UGameObject::SetAttackCollSize( const FVector& InSize )
{
	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
		attackColl->SetBoxExtent( InSize );
}

void UGameObject::SetAttackCollPos( const FVector& InPos )
{
	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
		attackColl->SetRelativeLocation( InPos );
}

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
	if( auto boxComponent = Cast<UBoxComponent>( OtherComp ) )
	{
		if( !boxComponent->GetName().Equals( TEXT( "AttackColl" ) ) )
			return;
	}
	else
	{ 
		return;
	}

	FString str = OwningCharacter->GetName() + TEXT( " : HitColl BeginOverlap! (HP : " ) + FString::FromInt( (int)Hp );
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), 1.f );
}

void UGameObject::_AnimStateChange()
{
	if( !OwningCharacter )
		return;

	if( !OwningCharacter->GetMesh()->GetAnimInstance() )
		return;

	auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	if( curMontage )
	{
		FString curAnimName = curMontage->GetName();
		if(      curAnimName.Equals( "MTG_Roll" ) )   AnimState = EAnimState::ROLL;
		else if( curAnimName.Equals( "MTG_Punch1" ) ) AnimState = EAnimState::PUNCH1;
		else if( curAnimName.Equals( "MTG_Punch2" ) ) AnimState = EAnimState::PUNCH2;
	}
	else
	{
		auto moveComponent = OwningCharacter->GetMovementComponent();
		if( moveComponent )
			moveComponent->IsFalling() ? AnimState = EAnimState::JUMP : AnimState = EAnimState::IDLE_RUN;
	}
}

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

void UGameObject::_ResetInfo()
{
	if( AnimState==EAnimState::IDLE_RUN || 
	    AnimState==EAnimState::JUMP || 
	    AnimState==EAnimState::DIE )
	{
		SetIsAttackMove( false );
		SetIsEnabledAttackColl( false );
	}
}
