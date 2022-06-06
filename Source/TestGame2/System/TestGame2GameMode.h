// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGame2GameMode.generated.h"

UCLASS(minimalapi)
class ATestGame2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestGame2GameMode();
	virtual void BeginPlay() override final;
	virtual void BeginDestroy() override final;
	virtual void Tick( float InDeltaTime ) override final;
};



