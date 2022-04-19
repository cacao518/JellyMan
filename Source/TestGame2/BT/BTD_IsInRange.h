// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UBTD_IsInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ExposeOnSpawn = true ) )
	float Range;

public:
	UBTD_IsInRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
