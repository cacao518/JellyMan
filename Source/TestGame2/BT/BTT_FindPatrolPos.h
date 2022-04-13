// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UBTT_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

};
