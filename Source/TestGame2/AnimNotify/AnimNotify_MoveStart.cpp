// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MoveStart.h"
#include "../Component/GameObject.h"

FString UAnimNotify_MoveStart::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_MoveStart::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UGameObject* obj = Cast<UGameObject>( MeshComp->GetOwner()->FindComponentByClass<UGameObject>() );
	if( !obj ) return;

	obj->SetMovePos( MovePower );
	obj->SetIsAttackMove( true );
}