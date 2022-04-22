// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect.h"
#include "GameFramework/Character.h"
#include "../System/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT( "Detect" );
	Interval = 1.0f;
}

void UBTS_Detect::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !controllingPawn )
		return;

	UWorld* world = controllingPawn->GetWorld();
	if( !world )
		return;

	FVector center = controllingPawn->GetActorLocation();

	// 600�� �������� ���� ��ü�� ���� ������Ʈ�� �����Ѵ�.
	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam( NAME_None, false, controllingPawn );
	bool bResult = world->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere( DetectRadius ),
		collisionQueryParam
	);

	// ������Ʈ�� ������ �Ǹ�, �� ������Ʈ�� Character���� �˻��Ѵ�.
	if( bResult )
	{
		for( FOverlapResult overlapResult : overlapResults )
		{
			ACharacter* character = Cast<ACharacter>( overlapResult.GetActor() );
			if( character&&character->GetController()->IsPlayerController() )
			{
				// Character��, �����忡 �����Ѵ�.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, character );

				// ����� ��.
				DrawDebugSphere( world, center, DetectRadius, 16, FColor::Green, false, 0.2f );
				DrawDebugPoint( world, character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f );
				DrawDebugLine( world, controllingPawn->GetActorLocation(), character->GetActorLocation(), FColor::Blue, false, 0.2f );
				return;
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, nullptr );
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject( AMonsterAIController::TargetKey, nullptr );
	}

	DrawDebugSphere( world, center, DetectRadius, 16, FColor::Red, false, 0.2f );
}