// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AMonsterAIController::HomePosKey( TEXT( "HomePos" ) );
const FName AMonsterAIController::PatrolPosKey( TEXT( "PatrolPos" ) );


AMonsterAIController::AMonsterAIController( FObjectInitializer const& object_initializer )
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject( TEXT( "BehaviorTree'/Game/Monster/BT_Monster.BT_Monster'" ) );
	if( BTObject.Succeeded() )
	{
		BTAsset = BTObject.Object;
	}

	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>( this, TEXT( "BlackboardComp" ) );
}

void AMonsterAIController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );
	if( blackboard )
	{
		//비헤이비어트리에 있는 블랙보드로 초기화
		blackboard->InitializeBlackboard( *BTAsset->BlackboardAsset );
	}

	RunAI();
}

void AMonsterAIController::RunAI()
{
	RunBehaviorTree( BTAsset );
}

void AMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>( BrainComponent );
	if( !BehaviorTreeComponent )
		return;

	BehaviorTreeComponent->StopTree( EBTStopMode::Safe );
}