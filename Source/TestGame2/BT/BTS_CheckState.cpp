// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckState.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "../Component/GameObject.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CheckState::UBTS_CheckState()
{
	NodeName = TEXT( "CheckState" );
	Interval = 0.1f;
}

void UBTS_CheckState::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return;

	auto gameObject = controllingPawn ? Cast<UGameObject>( controllingPawn->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	if( gameObject->GetAnimState() != EAnimState::COMMON_ACTION && OwnerComp.GetBlackboardComponent()->GetValueAsBool( AMonsterAIController::IsCommonActionKey ) )
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( AMonsterAIController::IsCommonActionKey, false );
	else if( gameObject->GetAnimState() == EAnimState::COMMON_ACTION && !OwnerComp.GetBlackboardComponent()->GetValueAsBool( AMonsterAIController::IsCommonActionKey ) )
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( AMonsterAIController::IsCommonActionKey, true );

	return;
}