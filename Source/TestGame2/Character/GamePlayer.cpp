// Copyright Epic Games, Inc. All Rights Reserved.

#include "GamePlayer.h"
#include "../System/MyPlayerController.h"
#include "../System/MyAnimInstance.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../Component/WeaponChange.h"
#include "../Manager/DataInfoManager.h"
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
	InputTypeAndFuncMap[ EInputKeyType::BOTH_MOUSE ]  = bind( &AGamePlayer::ProcessBothMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::SPACE       ] = bind( &AGamePlayer::ProcessSpace,      this );

	_ResetReadySkill();
}

void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void AGamePlayer::Tick( float InDeltaTime )
{
	Super::Tick(InDeltaTime);

	_ProcessReadySkill( InDeltaTime );
	_ProcessLockOn();
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
			result = PunchLeftStart();
			break;
		}
		case EWeaponState::SWORD:
		{
			result = SwordLeftStart();
			break;
		}
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
			result = PunchRightStart();
			break;
		}
		case EWeaponState::SWORD:
		{
			result = SwordRightStart();
			break;
		}
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::RIGHT_MOUSE );
}

void AGamePlayer::ProcessBothMouse()
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
	}
}

void AGamePlayer::ProcessWheel()
{
	if( LockOnTarget )
	{
		_LockOnRelease();
		return;
	}

	_LockOnStart();
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
		GameObject->SkillPlay( 4 );
	}
}

void AGamePlayer::ProcessR()
{
	if( !MatProperty )
		return;

	if( GameObject && GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		if( MatProperty->GetMatState() != EMaterialState::JELLY )
		{
			GameObject->SkillPlay( 10 );
			MatProperty->SetMatState();
		}
	}
}

void AGamePlayer::Process1()
{
	if( !WeaponChange )
		return;

	if( !( WeaponChange->CanWeaponChange( EWeaponState::SWORD ) ) )
		return;

	GameObject->SkillPlay( 5, GameObject->GetStat().AttackSpeed );
}

bool AGamePlayer::PunchLeftStart()
{
	if( !GameObject )
		return false;

	if( LockOnTarget )
		GameObject->LookAt( LockOnTarget );

	if( GameObject->GetCurMontageName().Equals( "MTG_Punch1" ) && GameObject->GetIsEnableDerivedKey() )
	{
		GameObject->SkillPlay( 9, GameObject->GetStat().AttackSpeed );
		return true;
	}
	else if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 2, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::PunchRightStart()
{
	if( !GameObject )
		return false;

	if( LockOnTarget )
		GameObject->LookAt( LockOnTarget );

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN ||
		GameObject->GetCurMontageName().Equals( "MTG_Punch1" ) ||
		GameObject->GetCurMontageName().Equals( "MTG_Punch3" ) )
	{
		GameObject->SkillPlay( 3, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::SwordLeftStart()
{
	if( !GameObject )
		return false;

	if( LockOnTarget )
		GameObject->LookAt( LockOnTarget );

	if( GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" ) && GameObject->GetIsEnableDerivedKey() )
	{
		GameObject->SkillPlay( 7, GameObject->GetStat().AttackSpeed );
		return true;
	}
	else if( GameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		GameObject->SkillPlay( 6, GameObject->GetStat().AttackSpeed );
		return true;
	}

	return false;
}

bool AGamePlayer::SwordRightStart()
{
	if( !GameObject )
		return false;

	if( LockOnTarget )
		GameObject->LookAt( LockOnTarget );

	if( GameObject->GetAnimState() == EAnimState::IDLE_RUN
		|| GameObject->GetCurMontageName().Equals( "MTG_SwordAttack1" )
		|| GameObject->GetCurMontageName().Equals( "MTG_SwordAttack2" ) )
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 시작
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_LockOnStart()
{
	UWorld* world = GetWorld();
	if( !world )
		return;

	FVector center = GetActorLocation();

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam( NAME_None, false, this );
	bool bResult = world->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere( Const::LOCKON_RANGE ),
		collisionQueryParam
	);

	if( !bResult )
		return;

	for( FOverlapResult overlapResult : overlapResults )
	{
		ACharacter* character = Cast<ACharacter>( overlapResult.GetActor() );
		if( !character )
			continue;

		if( auto widgetComp = Cast<UWidgetComponent>( character->GetDefaultSubobjectByName( TEXT( "LockOnMark" ) ) ) )
		{
			if( auto userWidget = Cast<UUserWidget>( widgetComp->GetUserWidgetObject() ) )
			{
				FOutputDeviceNull ar;
				userWidget->CallFunctionByNameWithArguments( TEXT( "AnimStart" ), ar, NULL, true );
				userWidget->SetVisibility( ESlateVisibility::SelfHitTestInvisible );

				LockOnTarget = character;
				CameraBoom->CameraRotationLagSpeed = Const::LOCKON_CAMERA_ROTAION_LAG_SPEED;
				return;
			}
		}
	}

	LockOnTarget = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 해제
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_LockOnRelease()
{
	if( auto widgetComp = Cast<UWidgetComponent>( LockOnTarget->GetDefaultSubobjectByName( TEXT( "LockOnMark" ) ) ) )
	{
		if( auto userWidget = Cast<UUserWidget>( widgetComp->GetUserWidgetObject() ) )
			userWidget->SetVisibility( ESlateVisibility::Collapsed );
	}

	LockOnTarget = nullptr;
	CameraBoom->CameraRotationLagSpeed = 0.f;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 기능 수행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AGamePlayer::_ProcessLockOn()
{
	if( !LockOnTarget )
		return;

	if( !GetValid( LockOnTarget ) || LockOnTarget->GetDistanceTo( this ) > Const::LOCKON_RANGE )
	{
		_LockOnRelease();
		return;
	}

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( GetActorLocation(), LockOnTarget->GetActorLocation() );
	rotator.Pitch = GetControlRotation().Pitch;
	GetController()->SetControlRotation( rotator );
}
