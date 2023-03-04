// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UGameUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UProgressBar* ProgressBarHP;
	class UProgressBar* ProgressBarJelly;

public:
	// NativeConstruct
	virtual void NativeConstruct() override;

	// BeginDestroy
	virtual void BeginDestroy() override;

};
