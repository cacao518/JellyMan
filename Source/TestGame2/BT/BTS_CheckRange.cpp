// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckRange.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "../Component/CharacterComp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTS_CheckRange::UBTS_CheckRange()
{
	NodeName = TEXT( "CheckRange" );
	Interval = 0.1f;
}

void UBTS_CheckRange::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return;

	ACharacter* target = Cast<ACharacter>( OwnerComp.GetBlackboardComponent()->GetValueAsObject( AMonsterAIController::TargetKey ) );
	if( !target )
		return;

	auto characterComp = controllingPawn ? Cast<UCharacterComp>( controllingPawn->FindComponentByClass<UCharacterComp>() ) : nullptr;
	if( !characterComp )
		return;

	for( auto& skill : characterComp->GetSkillInfos() )
	{
		if( characterComp->IsCoolingSkill( skill.Num ) )
			continue;

		if( target->GetDistanceTo( controllingPawn ) <= skill.ActivateRangeMax &&
			target->GetDistanceTo( controllingPawn ) >= skill.ActivateRangeMin )
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt( AMonsterAIController::CurSkillNumKey, skill.Num );
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt( AMonsterAIController::CurSkillNumKey, 0 );
	return;
}