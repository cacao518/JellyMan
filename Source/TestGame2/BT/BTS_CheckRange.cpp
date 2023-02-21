// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckRange.h"
#include "GameFramework/Character.h"
#include "../Actor/CharacterNPC.h"
#include "../System/MonsterAIController.h"
#include "../Manager/DataInfoManager.h"
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

	ACharacterNPC* controllingNPC = Cast< ACharacterNPC> ( OwnerComp.GetAIOwner()->GetPawn() );
	if( !controllingNPC )
		return;

	ACharacter* target = Cast<ACharacter>( OwnerComp.GetBlackboardComponent()->GetValueAsObject( AMonsterAIController::TargetKey ) );
	if( !target )
		return;

	auto characterComp = controllingNPC ? Cast<UCharacterComp>( controllingNPC->FindComponentByClass<UCharacterComp>() ) : nullptr;
	if( !characterComp )
		return;

	for( const auto& skillNum : controllingNPC->SkillInfos )
	{
		const auto& skillInfo = GetDataInfoManager().GetSkillInfos().Find( skillNum );
		if ( !skillInfo )
			continue;

		if( characterComp->IsCoolingSkill( skillNum ) )
			continue;

		if( target->GetDistanceTo( controllingNPC ) <= skillInfo->ActivateRangeMax &&
			target->GetDistanceTo( controllingNPC ) >= skillInfo->ActivateRangeMin )
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt( AMonsterAIController::CurSkillNumKey, skillNum );
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt( AMonsterAIController::CurSkillNumKey, 0 );
	return;
}