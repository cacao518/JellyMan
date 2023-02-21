// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AMonsterAIController::PatrolPosKey( TEXT( "PatrolPos" ) );
const FName AMonsterAIController::TargetKey( TEXT( "Target" ) );
const FName AMonsterAIController::CurSkillNumKey( TEXT( "CurSkillNum" ) );
const FName AMonsterAIController::IsCommonActionKey( TEXT( "IsCommonAction" ) );


AMonsterAIController::AMonsterAIController( FObjectInitializer const& object_initializer )
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject( TEXT( "BehaviorTree'/Game/Blueprints/Monster/BT_Monster.BT_Monster'" ) );
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
		//�����̺��Ʈ���� �ִ� ������� �ʱ�ȭ
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