// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();

	virtual void OnPossess( APawn* InPawn ) override;

	void RunAI();
	void StopAI();

	void SetBT( UBehaviorTree* InBT ) { BTAsset = InBT; };
	void SetBB( UBlackboardData* InBB ) { BBAsset = InBB; };

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
