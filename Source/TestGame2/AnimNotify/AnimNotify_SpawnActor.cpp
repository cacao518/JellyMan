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

	switch( Type )
	{
		case EObjectType::PC :
		{
			break;
		}
		case EObjectType::NPC : 
		{
			break;
		}
		case EObjectType::PROJECTILE :
		{
			break;
		}
		case EObjectType::GROUND_OBJECT :
		{
			break;
		}
	}

	GetObjectManager().SpawnActor( Actor, MeshComp->GetOwner()->GetActorLocation(), MeshComp->GetOwner()->GetActorRotation() );
}