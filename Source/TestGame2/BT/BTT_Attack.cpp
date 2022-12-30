// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "../ETC/SDB.h"
#include "../System/MonsterAIController.h"
#include "../Component/CharacterComp.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
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

	auto characterComp = controllingPawn ? Cast<UCharacterComp>( controllingPawn->FindComponentByClass<UCharacterComp>() ) : nullptr;
	if( !characterComp )
		return EBTNodeResult::Failed;

	characterComp->LookAt( target );

	bool result = characterComp->SkillPlay( OwnerComp.GetBlackboardComponent()->GetValueAsInt( AMonsterAIController::CurSkillNumKey ) );

	OwnerComp.GetBlackboardComponent()->SetValueAsInt( AMonsterAIController::CurSkillNumKey, 0 );

	return result ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}