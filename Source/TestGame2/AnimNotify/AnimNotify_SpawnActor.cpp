// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_SpawnActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Manager/ObjectManager.h"

FString UAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnActor::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	auto spawnPosComp = Cast<USceneComponent>( MeshComp->GetOwner()->GetDefaultSubobjectByName( TEXT( "SpawnPosComp" ) ) );
	if( !spawnPosComp )
		return;

	FVector relativeSpawnPos = spawnPosComp->GetRelativeLocation() + Pos;
	FVector worldSpawnPos = UKismetMathLibrary::TransformLocation( spawnPosComp->GetComponentTransform(), relativeSpawnPos );

	if( ObjectManager::IsVaild() )
		GetObjectManager().SpawnActor( Actor, worldSpawnPos, MeshComp->GetOwner()->GetActorRotation() + Rotate );
}