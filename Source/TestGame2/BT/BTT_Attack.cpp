// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "../ETC/SDB.h"
#include "../System/MonsterAIController.h"
#include "../Component/GameObject.h"
#include "GameFramework/Character.h"
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

	ACharacter* target = Cast<ACharacter>( OwnerComp.GetBlackboardComponent()->GetValueAsObject( AMonsterAIController::TargetKey ) );
	if( !target )
		return EBTNodeResult::Failed;

	auto gameObject = controllingPawn ? Cast<UGameObject>( controllingPawn->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return EBTNodeResult::Failed;

	for( auto skill : gameObject->GetSkillInfos() )
	{
		if( target->GetDistanceTo( controllingPawn ) >= skill.ActivateRangeMax || 
			target->GetDistanceTo( controllingPawn ) <= skill.ActivateRangeMin )
			continue;

		if( gameObject->GetAnimState() == EAnimState::IDLE_RUN )
		{
			gameObject->MontagePlay( skill.Anim );
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}