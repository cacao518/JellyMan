// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsCommonAction.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "../Component/GameObject.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsCommonAction::UBTD_IsCommonAction()
{
	NodeName = TEXT("IsCommonAction");
}

bool UBTD_IsCommonAction::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return false;

	auto gameObject = controllingPawn ? Cast<UGameObject>( controllingPawn->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return false;

	bool bResult = gameObject->GetAnimState() == EAnimState::COMMON_ACTION;
	return bResult;
}