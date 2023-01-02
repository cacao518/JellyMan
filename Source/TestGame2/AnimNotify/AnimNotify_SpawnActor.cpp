// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_SpawnActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Manager/ObjectManager.h"
#include "../Component/ObjectComp.h"

FString UAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnActor::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	if( !ObjectManager::IsVaild() )
		return;

	UObjectComp* objComp = Cast<UObjectComp>( MeshComp->GetOwner()->FindComponentByClass<UObjectComp>() );
	if( !objComp )
		return;

	auto spawnPosComp = Cast<USceneComponent>( MeshComp->GetOwner()->GetDefaultSubobjectByName( TEXT( "SpawnPosComp" ) ) );
	if( !spawnPosComp )
		return;

	FVector relativeSpawnPos = spawnPosComp->GetRelativeLocation() + Pos;
	FVector worldSpawnPos = UKismetMathLibrary::TransformLocation( spawnPosComp->GetComponentTransform(), relativeSpawnPos );

	if( SetAsParentTeamType )
		GetObjectManager().SpawnActor( Actor, worldSpawnPos, MeshComp->GetOwner()->GetActorRotation() + Rotate, objComp->GetTeamType() );
	else
		GetObjectManager().SpawnActor( Actor, worldSpawnPos, MeshComp->GetOwner()->GetActorRotation() + Rotate );
}