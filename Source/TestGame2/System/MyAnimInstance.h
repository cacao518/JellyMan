// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	float        Speed;                   

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	bool         IsJump;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	bool         IsFly;
};
