

#include "MyPlayerController.h"
#include "../Component/CharacterComp.h"
#include "../Component/MaterialComp.h"
#include "../Component/WeaponComp.h"
#include "../Manager/LockOnManager.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Actor/CharacterPC.h"
#include "../System/MyAnimInstance.h"
#include "../System/MyGameInstance.h"
#include "../ETC/SDB.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPlayerController::AMyPlayerController()
{
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	MyPlayer = Cast<ACharacterPC>( GetPawn() );
	if( !MyPlayer )
		return;

	CharacterComp   = MyPlayer->CharacterComp;
	MatComp      = MyPlayer->MatComp;
	WeaponComp   = MyPlayer->WeaponComp;

	check( InputComponent );
	InputComponent->BindAxis( "MoveForward", this, &AMyPlayerController::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AMyPlayerController::MoveRight );
	InputComponent->BindAxis( "TurnRate", this, &AMyPlayerController::TurnAtRate );
	InputComponent->BindAxis( "LookUpRate", this, &AMyPlayerController::LookUpAtRate );

	InputComponent->BindAxis( "Turn", MyPlayer, &APawn::AddControllerYawInput );
	InputComponent->BindAxis( "LookUp", MyPlayer, &APawn::AddControllerPitchInput );

	InputComponent->BindAction( "Shift",      IE_Pressed, this, &AMyPlayerController::JumpStart );
	InputComponent->BindAction( "Shift",      IE_Released, this, &AMyPlayerController::JumpStop );

	InputComponent->BindAction( "Space",      IE_Pressed, this, &AMyPlayerController::ProcessSpace);
	InputComponent->BindAction( "Tab",        IE_Pressed, this, &AMyPlayerController::ProcessTab );
	InputComponent->BindAction( "F",          IE_Pressed, this, &AMyPlayerController::ProcessF );
	InputComponent->BindAction( "R",          IE_Pressed, this, &AMyPlayerController::ProcessR );
	InputComponent->BindAction( "LeftClick",  IE_Pressed, this, &AMyPlayerController::ProcessLeftMouse );
	InputComponent->BindAction( "RightClick", IE_Pressed, this, &AMyPlayerController::ProcessRightMouse );
	InputComponent->BindAction( "WheelClick", IE_Pressed, this, &AMyPlayerController::ProcessWheelClick );
	InputComponent->BindAction( "WheelUp",    IE_Pressed, this, &AMyPlayerController::ProcessWheelUp );
	InputComponent->BindAction( "WheelDown",  IE_Pressed, this, &AMyPlayerController::ProcessWheelDown );
	InputComponent->BindAction( "1",          IE_Pressed, this, &AMyPlayerController::Process1 );
	InputComponent->BindAction( "2",          IE_Pressed, this, &AMyPlayerController::Process2 );
	InputComponent->BindAction( "3",          IE_Pressed, this, &AMyPlayerController::Process3 );


	InputTypeAndFuncMap[ EInputKeyType::LEFT_MOUSE ]  = bind( &AMyPlayerController::ProcessLeftMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::RIGHT_MOUSE ] = bind( &AMyPlayerController::ProcessRightMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::SPACE ]       = bind( &AMyPlayerController::ProcessSpace, this );
	InputTypeAndFuncMap[ EInputKeyType::Tab ]         = bind( &AMyPlayerController::ProcessTab, this );

	_ResetReadySkill();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::Tick( float InDeltaTime )
{
	Super::Tick( InDeltaTime );

	_ProcessInputBothKey();
	_ProcessRotationRate();
	_ProcessReadySkill( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief MoveForward
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::MoveForward( float Value )
{
	if( !MyPlayer || !CharacterComp )
		return;

	UMyAnimInstance* animInstance = MyPlayer ? Cast<UMyAnimInstance>( MyPlayer->GetMesh()->GetAnimInstance() ) : nullptr;
	if( !animInstance )
		return;

	if( Value != 0.0f )
	{
		if( animInstance->AnimState    == EAnimState   ::IDLE_RUN ||
			animInstance->AnimState    == EAnimState   ::JUMP     ||
			animInstance->AnimSubState == EAnimSubState::MOVABLE  ||
			animInstance->AnimSubState == EAnimSubState::UPPER_LOWER_BLEND ||
			CharacterComp->IsMontageInitialTime() )
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation( 0, Rotation.Yaw, 0 );
			const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
			MyPlayer->AddMovementInput( Direction, Value );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief MoveRight
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::MoveRight( float Value )
{
	if( !MyPlayer || !CharacterComp )
		return;

	UMyAnimInstance* animInstance = MyPlayer ? Cast<UMyAnimInstance>( MyPlayer->GetMesh()->GetAnimInstance() ) : nullptr;
	if( !animInstance )
		return;

	if( Value != 0.0f )
	{
		if( animInstance->AnimState    == EAnimState   ::IDLE_RUN ||
			animInstance->AnimState    == EAnimState   ::JUMP     || 
			animInstance->AnimSubState == EAnimSubState::MOVABLE  ||
			animInstance->AnimSubState == EAnimSubState::UPPER_LOWER_BLEND ||
			CharacterComp->IsMontageInitialTime() )
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation( 0, Rotation.Yaw, 0 );
			const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
			MyPlayer->AddMovementInput( Direction, Value );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief TurnAtRate
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::TurnAtRate( float Rate )
{
	if( MyPlayer )
		MyPlayer->AddControllerYawInput( Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief LookUpAtRate
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::LookUpAtRate( float Rate )
{
	if( MyPlayer )
		MyPlayer->AddControllerPitchInput( Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief JumpStart
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::JumpStart()
{
	if ( MyPlayer && CharacterComp && CharacterComp->GetAnimState() == EAnimState::IDLE_RUN )
		MyPlayer->Jump();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief JumpStart
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::JumpStop()
{
	if( MyPlayer )
		MyPlayer->StopJumping();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 왼쪽 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessLeftMouse()
{
	if( !WeaponComp )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerWeaponSkillInfos().Find( WeaponComp->GetCurWeaponNum() );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->L_SkillNum ) ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::LEFT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 오른쪽 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessRightMouse()
{
	if( !WeaponComp )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerWeaponSkillInfos().Find( WeaponComp->GetCurWeaponNum() );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->R_SkillNum ) ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::RIGHT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 양쪽 클릭 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessBothMouseDown()
{
	if( !MatComp || !MyPlayer )
		return;

	switch( MatComp->GetMatState() )
	{
		case EMaterialState::GRASS:
		{
			UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( MyPlayer->GetMesh()->GetAnimInstance() );
			if( animInstance && animInstance->IsJump )
				animInstance->IsFly = true;
		}
		break;
		case EMaterialState::ROCK:
		{
			_SkillPlay( 14 );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 양쪽 클릭 취소 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessBothMouseUp()
{
	if( !MatComp || !MyPlayer )
		return;

	switch( MatComp->GetMatState() )
	{
		case EMaterialState::GRASS:
		{
			UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( MyPlayer->GetMesh()->GetAnimInstance() );
			if( animInstance && animInstance->IsJump )
				animInstance->IsFly = false;
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 휠 클릭 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheelClick()
{
	if( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnRelease();
		return;
	}

	GetLockOnManager().LockOnStart();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 휠 위로 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheelUp()
{
	if ( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnStart( ELockOnMode::Prev );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 휠 아래로 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheelDown()
{
	if ( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnStart( ELockOnMode::Next );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스페이스 키 실행 (구르기)
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessSpace()
{
	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::SPACE );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->SkillNum ) ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::SPACE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스페이스 키 실행 ( 무기 던지기 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessTab()
{
	if ( !WeaponComp )
		return;

	if ( WeaponComp->GetWeaponState() == EWeaponState::DEFAULT )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerWeaponSkillInfos().Find( WeaponComp->GetCurWeaponNum() );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->ThrowSkillNum ) ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::Tab );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief F 키 실행 ( 재질 변경 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessF()
{
	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::F );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->SkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief R 키 실행 ( 젤리 돌아오기 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessR()
{
	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::R );
	if ( !skillInfo )
		return;

	if ( MatComp && MatComp->GetMatState() != EMaterialState::JELLY )
	{
		if ( _SkillPlay( skillInfo->SkillNum ) )
		{
			MatComp->SetMatState( nullptr, true );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 1번키 실행 ( 무기 생성 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::Process1()
{
	if( !WeaponComp )
		return;

	if( !( WeaponComp->CanWeaponComp( EWeaponState::SWORD ) ) )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::Num1 );
	if ( !skillInfo )
		return;

	_SkillPlay( skillInfo->SkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 2번키 실행 ( 무기 생성 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::Process2()
{
	if( !WeaponComp )
		return;

	if( !( WeaponComp->CanWeaponComp( EWeaponState::AXE ) ) )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::Num2 );
	if( !skillInfo )
		return;

	_SkillPlay( skillInfo->SkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 3번키 실행 ( 무기 생성 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::Process3()
{
	if( !WeaponComp )
		return;

	if( !( WeaponComp->CanWeaponComp( EWeaponState::SPEAR ) ) )
		return;

	const auto& skillInfo = GetDataInfoManager().GetPlayerDefaultSkillInfos().Find( EInputKeyType::Num3 );
	if( !skillInfo )
		return;

	_SkillPlay( skillInfo->SkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬을 재생한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool AMyPlayerController::_SkillPlay( int InSkillNum )
{
	if( !CharacterComp )
		return false;

	return CharacterComp->SkillPlay( InSkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 발동 대기중 스킬 초기화
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_ResetReadySkill()
{
	ReadySkillFunc = nullptr;
	ReadySkillResetTime = 0.f;
	ReadySkillInputKey = EInputKeyType::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 발동 대기중 스킬 설정
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_SetReadySkill( EInputKeyType InReadyInputKey )
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
void AMyPlayerController::_ProcessReadySkill( float InDeltaTime )
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
void AMyPlayerController::_ProcessRotationRate()
{
	if( !MyPlayer )
		return;

	UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( MyPlayer->GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	if( animInstance->IsFly )
	{
		MyPlayer->GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::FLY_ROTATION_RATE, 0.0f );
		return;
	}
	else if( ReadySkillResetTime > 0 || CharacterComp->IsMontageInitialTime() )
	{
		MyPlayer->GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::READY_SKILL_ROTATION_RATE, 0.0f );
		return;
	}

	MyPlayer->GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 다중 키 입력을 받는다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_ProcessInputBothKey()
{
	if( !IsDownBothMoustButton && IsInputKeyDown( EKeys::LeftMouseButton ) && IsInputKeyDown( EKeys::RightMouseButton ) )
	{
		IsDownBothMoustButton = true;
		ProcessBothMouseDown();
	}
	else if( IsDownBothMoustButton && !IsInputKeyDown( EKeys::LeftMouseButton ) && !IsInputKeyDown( EKeys::RightMouseButton ) )
	{
		IsDownBothMoustButton = false;
		ProcessBothMouseUp();
	}
}
