// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	class AGamePlayer* MyPlayer;

	float BaseTurnRate;
	float BaseLookUpRate;

public:
	AMyPlayerController();

public:
	virtual void BeginPlay() override;

	void MoveForward( float Value );

	void MoveRight( float Value );

	void TurnAtRate( float Rate );

	void LookUpAtRate( float Rate );
};
