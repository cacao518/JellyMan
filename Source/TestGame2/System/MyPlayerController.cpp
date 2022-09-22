

#include "MyPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Component/GameObject.h"
#include "../Character/GamePlayer.h"
#include "../System/MyAnimInstance.h"
#include "../ETC/SDB.h"

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

	MyPlayer = Cast<AGamePlayer>( GetPawn() );
	if( !MyPlayer )
		return;

	check( InputComponent );
	InputComponent->BindAxis( "MoveForward", this, &AMyPlayerController::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AMyPlayerController::MoveRight );
	InputComponent->BindAxis( "TurnRate", this, &AMyPlayerController::TurnAtRate );
	InputComponent->BindAxis( "LookUpRate", this, &AMyPlayerController::LookUpAtRate );

	InputComponent->BindAxis( "Turn", MyPlayer, &APawn::AddControllerYawInput );
	InputComponent->BindAxis( "LookUp", MyPlayer, &APawn::AddControllerPitchInput );

	InputComponent->BindAction( "Shift", IE_Pressed, MyPlayer, &ACharacter::Jump );
	InputComponent->BindAction( "Shift", IE_Released, MyPlayer, &ACharacter::StopJumping );

	InputComponent->BindAction( "Space", IE_Pressed, MyPlayer, &AGamePlayer::ProcessSpace );
	InputComponent->BindAction( "F", IE_Pressed, MyPlayer, &AGamePlayer::ProcessF );
	InputComponent->BindAction( "R", IE_Pressed, MyPlayer, &AGamePlayer::ProcessR );
	InputComponent->BindAction( "LeftClick", IE_Pressed, MyPlayer, &AGamePlayer::ProcessLeftMouse );
	InputComponent->BindAction( "RightClick", IE_Pressed, MyPlayer, &AGamePlayer::ProcessRightMouse );
	InputComponent->BindAction( "WheelClick", IE_Pressed, MyPlayer, &AGamePlayer::ProcessWheel );
	InputComponent->BindAction( "1", IE_Pressed, MyPlayer, &AGamePlayer::Process1 );
}

void AMyPlayerController::Tick( float InDeltaTime )
{
	Super::Tick( InDeltaTime );

	if( MyPlayer && IsInputKeyDown( EKeys::LeftMouseButton ) && IsInputKeyDown( EKeys::RightMouseButton ) )
		MyPlayer->ProcessBothMouse();

	_ProcessCameraUpdate( InDeltaTime );
}

void AMyPlayerController::MoveForward( float Value )
{
	if( !MyPlayer )
		return;

	auto gameObject = MyPlayer ? Cast<UGameObject>( MyPlayer->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	if( ( Value != 0.0f ) && ( gameObject->GetAnimState() == EAnimState::IDLE_RUN || gameObject->GetAnimState() == EAnimState::JUMP ) )
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		MyPlayer->AddMovementInput( Direction, Value );
	}
}

void AMyPlayerController::MoveRight( float Value )
{
	if( !MyPlayer )
		return;

	auto gameObject = MyPlayer ? Cast<UGameObject>( MyPlayer->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	if( ( Value!=0.0f ) && ( gameObject->GetAnimState() == EAnimState::IDLE_RUN || gameObject->GetAnimState() == EAnimState::JUMP ) )
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
		MyPlayer->AddMovementInput( Direction, Value );
	}
}

void AMyPlayerController::TurnAtRate( float Rate )
{
	if( MyPlayer )
		MyPlayer->AddControllerYawInput( Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() );
}

void AMyPlayerController::LookUpAtRate( float Rate )
{
	if( MyPlayer )
		MyPlayer->AddControllerPitchInput( Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 카메라 관련 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AMyPlayerController::_ProcessCameraUpdate( float InDeltaTime )
{
	UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( MyPlayer->GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	USpringArmComponent* cameraBoom = MyPlayer->CameraBoom;
	if( !cameraBoom )
		return;

	if( animInstance->IsFly )
	{
		cameraBoom->TargetArmLength = FMath::Lerp( cameraBoom->TargetArmLength, Const::FLY_TARGET_ARM_LENGTH, InDeltaTime * 10.f );
	}
	else
	{
		cameraBoom->TargetArmLength = FMath::Lerp( cameraBoom->TargetArmLength, Const::DEFAULT_TARGET_ARM_LENGTH, InDeltaTime * 5.f );
	}
}