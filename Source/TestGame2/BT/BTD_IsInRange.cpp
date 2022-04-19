// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInRange.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsInRange::UBTD_IsInRange()
{
	NodeName = TEXT("IsInRange");
}

bool UBTD_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !ControllingPawn )
		return false;

	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject( AMonsterAIController::TargetKey ) );
	if( !Target )
		return false;

	bool bResult = ( Target->GetDistanceTo(ControllingPawn ) <= Range );
	return bResult;
}