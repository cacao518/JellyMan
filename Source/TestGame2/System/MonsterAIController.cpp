// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMonsterAIController::AMonsterAIController()
{
	BTAsset = nullptr;
	BBAsset = nullptr;
}

void AMonsterAIController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );
	RunAI();
}

void AMonsterAIController::RunAI()
{
	UBlackboardComponent* BlackBoardComponent = Cast<UBlackboardComponent>( Blackboard );
	if( !BlackBoardComponent || !BBAsset || !BTAsset )
		return;

	if( UseBlackboard( BBAsset, BlackBoardComponent ) )
	{
		RunBehaviorTree( BTAsset );
	}
}

void AMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>( BrainComponent );
	if( !BehaviorTreeComponent )
		return;

	BehaviorTreeComponent->StopTree( EBTStopMode::Safe );
}