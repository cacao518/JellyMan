// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestGame2GameMode.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
#include "../Actor/CharacterPC.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATestGame2GameMode::ATestGame2GameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AMyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
}

void ATestGame2GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATestGame2GameMode::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATestGame2GameMode::Tick( float InDeltaTime )
{
	Super::Tick( InDeltaTime );

	UMyGameInstance* gameInstance = Cast<UMyGameInstance>( GetGameInstance() );
	if( !gameInstance )
		return;

	gameInstance->Tick( InDeltaTime );
}

