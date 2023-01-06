// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "../Character/CharacterNPC.h"
#include "../Component/CharacterComp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include <map>
#include <algorithm>

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT( "Detect" );
	Interval = 1.0f;
}

void UBTS_Detect::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	ACharacterNPC* controllingChar = Cast< ACharacterNPC >( OwnerComp.GetAIOwner()->GetPawn() );
	if( !controllingChar )
		return;

	auto characterComp = Cast<UCharacterComp>( controllingChar->FindComponentByClass<UCharacterComp>() );
	if( !characterComp )
		return;

	UWorld* world = controllingChar->GetWorld();
	if( !world )
		return;

	DetectRadius = controllingChar->DetectRange;

	FVector center = controllingChar->GetActorLocation();

	// 600�� �������� ���� ��ü�� ���� ������Ʈ�� �����Ѵ�.
	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam( NAME_None, false, controllingChar );
	bool bResult = world->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere( DetectRadius ),
		collisionQueryParam
	);

	if( !bResult )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, nullptr );
		return;
	}

	// ���� ����� ���� ���� �ǵ��� ����
	overlapResults.Sort( [controllingChar]( const auto& A, const auto& B ){
		return A.GetActor()->GetDistanceTo( controllingChar ) < B.GetActor()->GetDistanceTo( controllingChar );
		} );

	for( FOverlapResult overlapResult : overlapResults )
	{
		ACharacter* detectedChar = Cast<ACharacter>( overlapResult.GetActor() );
		if( !detectedChar )
			continue;

		auto detectedCharComp = Cast<UCharacterComp>( detectedChar->FindComponentByClass<UCharacterComp>() );
		if( !detectedCharComp )
			continue;

		if( detectedCharComp->GetTeamType() == characterComp->GetTeamType() )
			continue;

		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, detectedChar );
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, nullptr );

	// ����� ��.
	//DrawDebugSphere( world, center, DetectRadius, 16, FColor::Green, false, 0.2f );
	//DrawDebugPoint( world, detectedChar->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f );
	//DrawDebugLine( world, controllingPawn->GetActorLocation(), detectedChar->GetActorLocation(), FColor::Blue, false, 0.2f );

	/*DrawDebugSphere( world, center, DetectRadius, 16, FColor::Red, false, 0.2f );*/
}