// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindPatrolPos.h"
#include "../System/MonsterAIController.h"
#include "../Character/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTT_FindPatrolPos::UBTT_FindPatrolPos()
{
	NodeName = TEXT( "FindPatrolPos" );
}

EBTNodeResult::Type UBTT_FindPatrolPos::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return EBTNodeResult::Failed;

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem( controllingPawn->GetWorld() );
	if( !navSystem )
		return EBTNodeResult::Failed;

	if( AMonster* controllingMonster = Cast< AMonster >( OwnerComp.GetAIOwner()->GetPawn() ) )
		PatrolRadius = controllingMonster->PatrolRange;

	FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector( AMonsterAIController::HomePosKey );
	FNavLocation nextPatrol;
	if( navSystem->GetRandomPointInNavigableRadius( FVector::ZeroVector, PatrolRadius, nextPatrol ) )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector( AMonsterAIController::PatrolPosKey, nextPatrol.Location );
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}