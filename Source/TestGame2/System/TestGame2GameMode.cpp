// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestGame2GameMode.h"
#include "MyPlayerController.h"
#include "../GameObject/TestGame2Character.h"
#include "UObject/ConstructorHelpers.h"

ATestGame2GameMode::ATestGame2GameMode()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
