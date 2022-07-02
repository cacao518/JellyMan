// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckState.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UBTS_CheckState : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckState();

	virtual void TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;
};
