// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestGame2Character.h"
#include "../ETC/SDB.h"
#include "../Component/GameObject.h"
#include "Animation/AnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

using namespace Const;

//////////////////////////////////////////////////////////////////////////
// ATestGame2Character

ATestGame2Character::ATestGame2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create a GameObejct Component
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );
	GameObject->SetHp         ( Const::PLAYER_HP                   );
	GameObject->SetMoveSpeed  ( Const::PLAYER_DEFAULT_MOVE_SPEED   );
	GameObject->SetAttackSpeed( Const::PLAYER_DEFAULT_ATTACK_SPEED );

	// HitBox Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( GetMesh() );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );

	// AttackBox Component
	AttackColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "AttackColl" ) );
	AttackColl->SetupAttachment( GetMesh() );
	AttackColl->SetCollisionProfileName( TEXT( "AttackColl" ) );
}

void ATestGame2Character::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void ATestGame2Character::Tick( float InDeltaTime )
{
	Super::Tick(InDeltaTime);
}

void ATestGame2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis( "MoveForward", this, &ATestGame2Character::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ATestGame2Character::MoveRight );

	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "TurnRate", this, &ATestGame2Character::TurnAtRate );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );
	PlayerInputComponent->BindAxis( "LookUpRate", this, &ATestGame2Character::LookUpAtRate );

	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &ATestGame2Character::_RollStart);
	PlayerInputComponent->BindAction( "F", IE_Pressed, this, &ATestGame2Character::_TakeDownStart );
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ATestGame2Character::_Punch1Start );
	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &ATestGame2Character::_Punch2Start );
}

void ATestGame2Character::MoveForward( float Value )
{
	if( ( Controller!=nullptr )&&( Value!=0.0f ) && GameObject && GameObject->AnimState == EAnimState::IDLE_RUN )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		AddMovementInput( Direction, Value );
	}
}

void ATestGame2Character::MoveRight( float Value )
{
	if( ( Controller!=nullptr )&&( Value!=0.0f ) && GameObject && GameObject->AnimState == EAnimState::IDLE_RUN )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
		AddMovementInput( Direction, Value );
	}
}

void ATestGame2Character::TurnAtRate( float Rate )
{
	AddControllerYawInput( Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds() );
}

void ATestGame2Character::LookUpAtRate( float Rate )
{
	AddControllerPitchInput( Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds() );
}

void ATestGame2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		//Jump();
}

void ATestGame2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		//StopJumping();
}

void ATestGame2Character::Jump()
{
	if( GameObject && GameObject->AnimState != EAnimState::IDLE_RUN )
		return;

	Super::Jump();
}

void ATestGame2Character::_RollStart()
{
	if( GameObject && GameObject->AnimState != EAnimState::IDLE_RUN )
		return;

	GameObject->ResetInfo( true );
	GameObject->MontagePlay( RollAnimation );
}

void ATestGame2Character::_Punch1Start()
{
	if( GameObject && GameObject->AnimState != EAnimState::IDLE_RUN )
		return;

	GameObject->ResetInfo( true );
	GameObject->MontagePlay( Punch1Animation, GameObject->AttackSpeed );
}

void ATestGame2Character::_Punch2Start()
{
	if( GameObject && 
	    GameObject->AnimState != EAnimState::IDLE_RUN && 
	    !GameObject->GetCurMontageName().Equals( "MTG_Punch1" ) )
		return;

	GameObject->ResetInfo( true );
	GameObject->MontagePlay( Punch2Animation, GameObject->AttackSpeed );
}

void ATestGame2Character::_TakeDownStart()
{
	if( GameObject&&
		GameObject->AnimState!=EAnimState::IDLE_RUN )
		return;

	GameObject->ResetInfo( true );
	GameObject->MontagePlay( TakeDownAnimation );
}


