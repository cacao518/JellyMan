// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterial.h"
#include "../Component/GameObject.h"

FString UAnimNotify_CopyMaterial::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterial::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UGameObject* obj = Cast<UGameObject>( MeshComp->GetOwner()->FindComponentByClass<UGameObject>() );
	if( !obj ) return;
}