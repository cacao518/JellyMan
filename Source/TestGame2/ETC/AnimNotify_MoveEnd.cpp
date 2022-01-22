// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MoveEnd.h"
#include "../Component/GameObject.h"

FString UAnimNotify_MoveEnd::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_MoveEnd::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UGameObject* obj = Cast<UGameObject>( MeshComp->GetOwner()->FindComponentByClass<UGameObject>());
	if( !obj ) return;

	obj->SetIsAttackMove( false );
}