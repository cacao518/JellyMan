// Copyright Epic Games, Inc. All Rights Reserved.

#include "GamePlayer.h"
#include "../ETC/SDB.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../Component/WeaponChange.h"
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
// AGamePlayer

AGamePlayer::AGamePlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create a GameObejct Component
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );

	// Create a MaterialProperty Component
	MatProperty = CreateDefaultSubobject<UMaterialProperty>( TEXT( "MatProperty" ) );

	// Create a WeaponChange Component
	WeaponChange = CreateDefaultSubobject<UWeaponChange>( TEXT( "WeaponChange" ) );

	// HitBox Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( GetMesh() );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );

	// AttackBox Component
	AttackColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "AttackColl" ) );
	AttackColl->SetupAttachment( GetMesh() );
	AttackColl->SetCollisionProfileName( TEXT( "AttackColl" ) );

	// TileBox Component
	TileColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "TileColl" ) );
	TileColl->SetupAttachment( GetMesh() );
	TileColl->SetCollisionProfileName( TEXT( "TileColl" ) );
}

void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void AGamePlayer::Tick( float InDeltaTime )
{
	Super::Tick(InDeltaTime);
}

void AGamePlayer::Jump()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		Super::Jump();
}

void AGamePlayer::LeftAttack()
{
	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			Punch1Start();
			break;
		}
		case EWeaponState::SWORD:
		{
		   if( GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" ) && GameObject->GetIsEnableDerivedKey() )
				SwordAttack2Start();
			else
				SwordAttack1Start();
			break;
		}
		default:
			break;
	}
}

void AGamePlayer::RightAttack()
{
	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
	case EWeaponState::MAX:
	{
		Punch2Start();
		break;
	}
	case EWeaponState::SWORD:
	{
		SwordAttack3Start();
		break;
	}
	default:
		break;
	}
}

void AGamePlayer::RollStart()
{
	if( !GameObject )
		return;
	
	if( GameObject->GetCurMontageName().Equals( "MTG_Roll" ) )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN || GameObject->GetAnimState() == EAnimState::COMMON_ACTION )
		GameObject->SkillPlay( 1, GameObject->GetStat().MoveSpeed );
}

void AGamePlayer::Punch1Start()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		GameObject->SkillPlay( 2, GameObject->GetStat().AttackSpeed );
}

void AGamePlayer::Punch2Start()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN || GameObject->GetCurMontageName().Equals( "MTG_Punch1" ) )
		GameObject->SkillPlay( 3, GameObject->GetStat().AttackSpeed );
}

void AGamePlayer::TakeDownStart()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		GameObject->SkillPlay( 4, GameObject->GetStat().AttackSpeed );
}

void AGamePlayer::EquipSword()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		GameObject->SkillPlay( 5, GameObject->GetStat().AttackSpeed );
}

void AGamePlayer::SwordAttack1Start()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		GameObject->SkillPlay( 6, GameObject->GetStat().AttackSpeed );
}

void AGamePlayer::SwordAttack2Start()
{
	if( GameObject )
		GameObject->SkillPlay( 7, GameObject->GetStat().AttackSpeed );
}


void AGamePlayer::SwordAttack3Start()
{
	if( !GameObject )
		return;

	if(    GameObject->GetAnimState() == EAnimState::IDLE_RUN
	    || GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" )
		|| GameObject->GetCurMontageName().Equals( "MTG_SwordAttack2" ) )
		GameObject->SkillPlay( 8, GameObject->GetStat().AttackSpeed );
}