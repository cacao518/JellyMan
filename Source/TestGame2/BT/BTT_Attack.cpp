// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "../System/MonsterAIController.h"
#include "../Component/GameObject.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT( "Attack" );
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return EBTNodeResult::Failed;

	auto gameObject = controllingPawn ? Cast<UGameObject>( controllingPawn->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return EBTNodeResult::Failed;

	if( !AttackAnim )
		return EBTNodeResult::Failed;

	if( gameObject->GetAnimState() == EAnimState::IDLE_RUN )
	{
		gameObject->MontagePlay( AttackAnim );
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}