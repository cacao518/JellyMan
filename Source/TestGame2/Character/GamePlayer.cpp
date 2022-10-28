// Copyright Epic Games, Inc. All Rights Reserved.

#include "GamePlayer.h"
#include "../System/MyPlayerController.h"
#include "../System/MyAnimInstance.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../Component/WeaponChange.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/LockOnManager.h"
#include "Animation/AnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


using namespace Const;


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
	CameraBoom->TargetArmLength = Const::DEFAULT_TARGET_ARM_LENGTH;
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief BeginPlay
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::Tick( float InDeltaTime )
{
	Super::Tick(InDeltaTime);

	_ProcessRotationRate();
	_ProcessReadySkill( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 점프 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::Jump()
{
	if( !GameObject )
		return;

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
		Super::Jump();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 왼쪽 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessLeftMouse()
{
	bool result = false;

	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			result = GameObject->SkillPlay( 2 );
			break;
		}
		case EWeaponState::SWORD:
		{
			result = GameObject->SkillPlay( 6 );
			break;
		}
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::LEFT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 오른쪽 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessRightMouse()
{
	bool result = false;

	if( !GameObject )
		return;

	switch( WeaponChange->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			result = GameObject->SkillPlay( 3 );
			break;
		}
		case EWeaponState::SWORD:
		{
			result = GameObject->SkillPlay( 8 );
			break;
		}
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::RIGHT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 양쪽 클릭 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessBothMouseDown()
{
	if( !MatProperty )
		return;

	switch( MatProperty->GetMatState() )
	{
		case EMaterialState::GRASS:
		{
			UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
			if( animInstance && animInstance->IsJump )
				animInstance->IsFly = true;
		}
		break;
		case EMaterialState::ROCK:
		{
			GameObject->SkillPlay( 11 );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 양쪽 클릭 취소 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessBothMouseUp()
{
	if( !MatProperty )
		return;

	switch( MatProperty->GetMatState() )
	{
		case EMaterialState::GRASS:
		{
			UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
			if( animInstance && animInstance->IsJump )
				animInstance->IsFly = false;
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 휠 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessWheel()
{
	if( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnRelease();
		return;
	}

	GetLockOnManager().LockOnStart();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스페이스 키 실행 (구르기)
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessSpace()
{
	bool result = false;

	if( GameObject )
		result = GameObject->SkillPlay( 1 );
	
	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::SPACE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief F 키 실행 ( 재질 변경 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessF()
{
	if( GameObject )
		GameObject->SkillPlay( 4 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief R 키 실행 ( 젤리 돌아오기 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::ProcessR()
{
	if( !MatProperty || !GameObject )
		return;

	if( MatProperty->GetMatState() != EMaterialState::JELLY )
	{
		if( GameObject->SkillPlay( 10 ) )
			MatProperty->SetMatState();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 1번키 실행 ( 무기 생성 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::Process1()
{
	if( !WeaponChange )
		return;

	if( !( WeaponChange->CanWeaponChange( EWeaponState::SWORD ) ) )
		return;

	GameObject->SkillPlay( 5 );
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
		_ResetReadySkill();
		return;
	}
	else
	{
		ReadySkillResetTime -= InDeltaTime;

		// 대기중인 스킬 발동
		if( ReadySkillFunc )
			ReadySkillFunc();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 회전 비율 값을 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_ProcessRotationRate()
{
	UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	if( animInstance->IsFly )
	{
		GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::FLY_ROTATION_RATE, 0.0f );
		return;
	} 
	else if( ReadySkillResetTime > 0 )
	{
		GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::READY_SKILL_ROTATION_RATE, 0.0f );
		return;
	}

	GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f );
}