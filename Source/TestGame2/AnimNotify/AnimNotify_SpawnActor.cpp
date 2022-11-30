// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_SpawnActor.h"
#include "../Manager/ObjectManager.h"

FString UAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnActor::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	if( ObjectManager::IsVaild() )
		GetObjectManager().SpawnActor( Actor, MeshComp->GetOwner()->GetActorLocation(), MeshComp->GetOwner()->GetActorRotation() + Rotate );
}