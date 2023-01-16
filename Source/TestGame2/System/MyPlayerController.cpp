

#include "MyPlayerController.h"
#include "../Component/CharacterComp.h"
#include "../Component/MaterialComp.h"
#include "../Component/WeaponComp.h"
#include "../Manager/LockOnManager.h"
#include "../Manager/ObjectManager.h"
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

	InputComponent->BindAction( "Shift",      IE_Pressed, MyPlayer, &ACharacter::Jump );
	InputComponent->BindAction( "Shift",      IE_Released, MyPlayer, &ACharacter::StopJumping );

	InputComponent->BindAction( "Space",      IE_Pressed, this, &AMyPlayerController::ProcessSpace);
	InputComponent->BindAction( "F",          IE_Pressed, this, &AMyPlayerController::ProcessF );
	InputComponent->BindAction( "R",          IE_Pressed, this, &AMyPlayerController::ProcessR );
	InputComponent->BindAction( "LeftClick",  IE_Pressed, this, &AMyPlayerController::ProcessLeftMouse );
	InputComponent->BindAction( "RightClick", IE_Pressed, this, &AMyPlayerController::ProcessRightMouse );
	InputComponent->BindAction( "WheelClick", IE_Pressed, this, &AMyPlayerController::ProcessWheelClick );
	InputComponent->BindAction( "WheelUp",    IE_Pressed, this, &AMyPlayerController::ProcessWheelUp );
	InputComponent->BindAction( "WheelDown",  IE_Pressed, this, &AMyPlayerController::ProcessWheelDown );
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
//// @brief ���콺 ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessLeftMouse()
{
	if( !WeaponComp )
		return;

	bool result = false;

	switch( WeaponComp->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			result = _SkillPlay( 2 );
			break;
		}
		case EWeaponState::SWORD:
		{
			result = _SkillPlay( 6, 12, 12 );
			break;
		}
	}
	
	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::LEFT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���콺 ������ ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessRightMouse()
{
	if( !WeaponComp )
		return;

	bool result = false;

	switch( WeaponComp->GetWeaponState() )
	{
		case EWeaponState::MAX:
		{
			result = _SkillPlay( 3 );
			break;
		}
		case EWeaponState::SWORD:
		{
			result = _SkillPlay( 8, 14, 14 );
			break;
		}
	}

	result ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::RIGHT_MOUSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���콺 ���� Ŭ�� ����
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
			_SkillPlay( 11 );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���콺 ���� Ŭ�� ��� ����
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
//// @brief ���콺 �� Ŭ�� ����
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
//// @brief ���콺 �� ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheelUp()
{
	if ( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnStart( ELockOnMode::Prev );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���콺 �� �Ʒ��� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessWheelDown()
{
	if ( GetLockOnManager().GetLockOnTarget() )
	{
		GetLockOnManager().LockOnStart( ELockOnMode::Next );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �����̽� Ű ���� (������)
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessSpace()
{
	_SkillPlay( 1 ) ? _ResetReadySkill() : _SetReadySkill( EInputKeyType::SPACE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief F Ű ���� ( ���� ���� )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessF()
{
	_SkillPlay( 4 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief R Ű ���� ( ���� ���ƿ��� )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::ProcessR()
{
	if( MatComp && MatComp->GetMatState() != EMaterialState::JELLY )
	{
		if( _SkillPlay( 10 ) )
			MatComp->SetMatState();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 1��Ű ���� ( ���� ���� )
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::Process1()
{
	if( !WeaponComp )
		return;

	if( !( WeaponComp->CanWeaponComp( EWeaponState::SWORD ) ) )
		return;

	_SkillPlay( 5 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ��ų�� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool AMyPlayerController::_SkillPlay( int InBasicSkillNum, int InMiddleSkillNum, int InHardSkillNum )
{
	if( !CharacterComp )
		return false;

	if( MatComp && InMiddleSkillNum != 0 && InHardSkillNum != 0 )
	{
		if( MatComp->IsHardIntensity() )
			return CharacterComp->SkillPlay( InHardSkillNum );
		else if( MatComp->IsMiddleIntensity() )
			return CharacterComp->SkillPlay( InMiddleSkillNum );
	}

	return CharacterComp->SkillPlay( InBasicSkillNum );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ߵ� ����� ��ų �ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_ResetReadySkill()
{
	ReadySkillFunc = nullptr;
	ReadySkillResetTime = 0.f;
	ReadySkillInputKey = EInputKeyType::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ߵ� ����� ��ų ����
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
//// @brief �ߵ� ������� ��ų ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_ProcessReadySkill( float InDeltaTime )
{
	// Ÿ�Ӿƿ� �ʱ�ȭ
	if( ReadySkillResetTime <= 0 )
	{
		_ResetReadySkill();
		return;
	}
	else
	{
		ReadySkillResetTime -= InDeltaTime;

		// ������� ��ų �ߵ�
		if( ReadySkillFunc )
			ReadySkillFunc();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ȸ�� ���� ���� �����Ѵ�.
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
//// @brief ���� Ű �Է��� �޴´�.
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
