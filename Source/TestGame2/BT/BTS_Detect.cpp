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

	// 600의 반지름을 가진 구체를 만들어서 오브젝트를 감지한다.
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

	map< ACharacter*, float > distanceMap;
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

		distanceMap[ detectedChar ] = detectedChar->GetDistanceTo( controllingChar );
	}

	// 가장 가까운 거리로 선택
	auto minDistanceChar = min_element( distanceMap.begin(), distanceMap.end(), []( const auto& a, const auto& b ) {
		return a.second < b.second;
		} );

	if( minDistanceChar != distanceMap.end() )
		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, minDistanceChar->first );
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, nullptr );

	// 디버깅 용.
	//DrawDebugSphere( world, center, DetectRadius, 16, FColor::Green, false, 0.2f );
	//DrawDebugPoint( world, detectedChar->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f );
	//DrawDebugLine( world, controllingPawn->GetActorLocation(), detectedChar->GetActorLocation(), FColor::Blue, false, 0.2f );

	/*DrawDebugSphere( world, center, DetectRadius, 16, FColor::Red, false, 0.2f );*/
}