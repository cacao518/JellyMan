// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsCommonAction.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UBTD_IsCommonAction : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IsCommonAction();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
