// Copyright Epic Games, Inc. All Rights Reserved.

#include "GamePlayer.h"
#include "../System/MyPlayerController.h"
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 280.0f; 
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

	InputTypeAndFuncMap[ EInputKeyType::LEFT_MOUSE  ] = bind( &AGamePlayer::ProcessLeftMouse,  this );
	InputTypeAndFuncMap[ EInputKeyType::RIGHT_MOUSE ] = bind( &AGamePlayer::ProcessRightMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::SPACE       ] = bind( &AGamePlayer::ProcessSpace,      this );

	_ResetReadySkill();
}

void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	if( MatProperty )
		MatProperty->SetMatState( EMaterialState::JELLY );
}

void AGamePlayer::Tick( float InDeltaTime )
{
	Super::Tick(InDeltaTime);

	_ProcessReadySkill( InDeltaTime );
}

void AGamePlayer::Jump()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		Super::Jump();
}

void AGamePlayer::ProcessLeftMouse()
{
	bool result = false;

	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			result = Punch1Start();
			break;
		}
		case EWeaponState::SWORD:
		{
		   if( GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" ) && GameObject->GetIsEnableDerivedKey() )
			   result = SwordAttack2Start();
			else
			   result = SwordAttack1Start();
			break;
		}
		default:
			break;
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::LEFT_MOUSE );
}

void AGamePlayer::ProcessRightMouse()
{
	bool result = false;

	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
	case EWeaponState::MAX:
	{
		result = Punch2Start();
		break;
	}
	case EWeaponState::SWORD:
	{
		result = SwordAttack3Start();
		break;
	}
	default:
		break;
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::RIGHT_MOUSE );
}

void AGamePlayer::ProcessSpace()
{
	bool result = false;

	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 1, GameObject->GetStat().MoveSpeed );
		result = true;
	}
	
	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::SPACE );
}

void AGamePlayer::ProcessF()
{
	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 4, GameObject->GetStat().AttackSpeed );
	}
}

void AGamePlayer::Process1()
{
	if( MatProperty->GetMatState() != EMaterialState::JELLY || WeaponChange->GetWeaponState() != EWeaponState::MAX )
		return;

	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 5, GameObject->GetStat().AttackSpeed );
	}
}

bool AGamePlayer::Punch1Start()
{
	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 2, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::Punch2Start()
{
	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN ||
		GameObject->GetCurMontageName().Equals( "MTG_Punch1" ) )
	{
		GameObject->SkillPlay( 3, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::SwordAttack1Start()
{
	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 6, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::SwordAttack2Start()
{
	if( GameObject )
	{
		GameObject->SkillPlay( 7, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}


bool AGamePlayer::SwordAttack3Start()
{
	if( GameObject &&
		( GameObject->GetAnimState() == EAnimState::IDLE_RUN
		|| GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" )
		|| GameObject->GetCurMontageName().Equals( "MTG_SwordAttack2" ) ) )
	{
		GameObject->SkillPlay( 8, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 발동 대기중 스킬 초기화
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_ResetReadySkill()
{
	ReadySkillFunc = nullptr;
	ReadySkillResetTime = 0.f;
	ReadySkillInputKey = EInputKeyType::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 발동 대기중 스킬 설정
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_SetReadySkill( EInputKeyType InReadyInputKey )
{
	if( ReadySkillInputKey == InReadyInputKey )
		return;

	ReadySkillFunc = InputTypeAndFuncMap[ InReadyInputKey ];
	ReadySkillResetTime = Const::READY_SKILL_DURATION;
	ReadySkillInputKey = InReadyInputKey;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 발동 대기중인 스킬 수행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_ProcessReadySkill( float InDeltaTime )
{
	// 타임아웃 초기화
	if( ReadySkillResetTime <= 0 )
	{
		GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f );
		_ResetReadySkill();
		return;
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::READY_SKILL_ROTATION_RATE, 0.0f );
		ReadySkillResetTime -= InDeltaTime;

		// 대기중인 스킬 발동
		if( ReadySkillFunc )
			ReadySkillFunc();
	}
}