// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObject.h"
#include "../ETC/SDB.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

UGameObject::UGameObject()
{
	PrimaryComponentTick.bCanEverTick = true;
	AttackCollSize = FVector( 0.f, 0.f, 0.f );
}

void UGameObject::BeginPlay()
{
	Super::BeginPlay();
}

void UGameObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_AnimStateChange();
	_Move();
	_ResetAttackColl();
}

void UGameObject::SetAttackCollSize( float InX, float InY, float InZ )
{
	AttackCollSize = FVector( InX, InY, InZ );

	ACharacter* OwningCharacter = Cast<ACharacter>( GetOwner() );
	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ): nullptr;
	if( attackColl )
		attackColl->SetBoxExtent( AttackCollSize );
}

void UGameObject::SetAttackCollSize( const FVector& InVector )
{
	AttackCollSize = InVector;

	ACharacter* OwningCharacter = Cast<ACharacter>( GetOwner() );
	auto attackColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
		attackColl->SetBoxExtent( AttackCollSize );
}

void UGameObject::_AnimStateChange()
{
	ACharacter* OwningCharacter = Cast<ACharacter>( GetOwner() );
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
	ACharacter* OwningCharacter = Cast<ACharacter>( GetOwner() );
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

void UGameObject::_ResetAttackColl()
{
	if( AnimState==EAnimState::IDLE_RUN || 
	    AnimState==EAnimState::JUMP || 
	    AnimState==EAnimState::DIE )
	{
		SetAttackCollSize( 0.f, 0.f, 0.f );
	}
}
