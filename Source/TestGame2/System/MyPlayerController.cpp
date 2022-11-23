

#include "MyPlayerController.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../Component/WeaponChange.h"
#include "../Manager/LockOnManager.h"
#include "../Character/CharacterPC.h"
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

	GameObject   = MyPlayer->GameObject;
	MatProperty  = MyPlayer->MatProperty;
	WeaponChange = MyPlayer->WeaponChange;

	check( InputComponent );
	InputComponent->BindAxis( "MoveForward", this, &AMyPlayerController::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AMyPlayerController::MoveRight );
	InputComponent->BindAxis( "TurnRate", this, &AMyPlayerController::TurnAtRate );
	InputComponent->BindAxis( "LookUpRate", this, &AMyPlayerController::LookUpAtRate );

	InputComponent->BindAxis( "Turn", MyPlayer, &APawn::AddControllerYawInput );
	InputComponent->BindAxis( "LookUp", MyPlayer, &APawn::AddControllerPitchInput );

	InputComponent->BindAction( "Shift",      IE_Pressed, MyPlayer, &ACharacter::Jump );
	InputComponent->BindAction( "Shift",      IE_Released, MyPlayer, &ACharacter::StopJumping );

	InputComponent->BindAction( "Space",      IE_Pressed, this, &AMyPlayerController::ProcessSpace);
	InputComponent->BindAction( "F",          IE_Pressed, this, &AMyPlayerController::ProcessF );
	InputComponent->BindAction( "R",          IE_Pressed, this, &AMyPlayerController::ProcessR );
	InputComponent->BindAction( "LeftClick",  IE_Pressed, this, &AMyPlayerController::ProcessLeftMouse );
	InputComponent->BindAction( "RightClick", IE_Pressed, this, &AMyPlayerController::ProcessRightMouse );
	InputComponent->BindAction( "WheelClick", IE_Pressed, this, &AMyPlayerController::ProcessWheel );
	InputComponent->BindAction( "1",          IE_Pressed, this, &AMyPlayerController::Process1 );

	InputTypeAndFuncMap[ EInputKeyType::LEFT_MOUSE ]  = bind( &AMyPlayerController::ProcessLeftMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::RIGHT_MOUSE ] = bind( &AMyPlayerController::ProcessRightMouse, this );
	InputTypeAndFuncMap[ EInputKeyType::SPACE ]       = bind( &AMyPlayerController::ProcessSpace, this );

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
	if( !MyPlayer )
		return;

	UMyAnimInstance* animInstance = MyPlayer ? Cast<UMyAnimInstance>( MyPlayer->GetMesh()->GetAnimInstance() ) : nullptr;
	if( !animInstance )
		return;

	if( Value != 0.0f )
	{
		if( animInstance->AnimState    == EAnimState   ::IDLE_RUN ||
			animInstance->AnimState    == EAnimState   ::JUMP     ||
			animInstance->AnimSubState == EAnimSubState::MOVABLE  ||
			animInstance->AnimSubState == EAnimSubState::UPPER_LOWER_BLEND )
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
	if( !MyPlayer )
		return;

	UMyAnimInstance* animInstance = MyPlayer ? Cast<UMyAnimInstance>( MyPlayer->GetMesh()->GetAnimInstance() ) : nullptr;
	if( !animInstance )
		return;

	if( Value != 0.0f )
	{
		if( animInstance->AnimState    == EAnimState   ::IDLE_RUN ||
			animInstance->AnimState    == EAnimState   ::JUMP     || 
			animInstance->AnimSubState == EAnimSubState::MOVABLE  ||
			animInstance->AnimSubState == EAnimSubState::UPPER_LOWER_BLEND )
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
//// @brief 마우스 왼쪽 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessLeftMouse()
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
void AMyPlayerController::ProcessRightMouse()
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
void AMyPlayerController::ProcessBothMouseDown()
{
	if( !MatProperty || !MyPlayer )
		return;

	switch( MatProperty->GetMatState() )
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
			GameObject->SkillPlay( 11 );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 마우스 양쪽 클릭 취소 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessBothMouseUp()
{
	if( !MatProperty || !MyPlayer )
		return;

	switch( MatProperty->GetMatState() )
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
//// @brief 마우스 휠 실행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheel()
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
void AMyPlayerController::ProcessSpace()
{
	bool result = false;

	if( GameObject )
		result = GameObject->SkillPlay( 1 );

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::SPACE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief F 키 실행 ( 재질 변경 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessF()
{
	if( GameObject )
		GameObject->SkillPlay( 4 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief R 키 실행 ( 젤리 돌아오기 )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessR()
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
void AMyPlayerController::Process1()
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
	else if( ReadySkillResetTime > 0 )
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
