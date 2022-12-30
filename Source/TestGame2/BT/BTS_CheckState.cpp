// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckState.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "../Component/CharacterComp.h"
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

	auto characterComp = controllingPawn ? Cast<UCharacterComp>( controllingPawn->FindComponentByClass<UCharacterComp>() ) : nullptr;
	if( !characterComp )
		return;

	if( characterComp->GetAnimState() != EAnimState::COMMON_ACTION && OwnerComp.GetBlackboardComponent()->GetValueAsBool( AMonsterAIController::IsCommonActionKey ) )
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( AMonsterAIController::IsCommonActionKey, false );
	else if( characterComp->GetAnimState() == EAnimState::COMMON_ACTION && !OwnerComp.GetBlackboardComponent()->GetValueAsBool( AMonsterAIController::IsCommonActionKey ) )
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( AMonsterAIController::IsCommonActionKey, true );

	return;
}