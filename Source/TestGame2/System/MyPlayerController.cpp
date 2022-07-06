

#include "MyPlayerController.h"
#include "../Component/GameObject.h"
#include "../Character/GamePlayer.h"
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

	InputComponent->BindAction( "Space", IE_Pressed, MyPlayer, &AGamePlayer::RollStart );
	InputComponent->BindAction( "F", IE_Pressed, MyPlayer, &AGamePlayer::TakeDownStart );
	InputComponent->BindAction( "LeftClick", IE_Pressed, MyPlayer, &AGamePlayer::LeftAttack );
	InputComponent->BindAction( "RightClick", IE_Pressed, MyPlayer, &AGamePlayer::RightAttack );
	InputComponent->BindAction( "1", IE_Pressed, MyPlayer, &AGamePlayer::EquipSword );
}

void AMyPlayerController::MoveForward( float Value )
{
	if( !MyPlayer )
		return;

	auto gameObject = MyPlayer ? Cast<UGameObject>( MyPlayer->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation( 0, Rotation.Yaw, 0 );
	const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
	gameObject->AddMoveDirectionInAction( Direction, Value );

	if( ( Value != 0.0f ) && gameObject && gameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
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

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation( 0, Rotation.Yaw, 0 );
	const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
	gameObject->AddMoveDirectionInAction( Direction, Value );

	if( ( Value!=0.0f )&& gameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
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
