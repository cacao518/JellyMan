// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DerivedKey.h"
#include "../Component/GameObject.h"

FString UAnimNotify_DerivedKey::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DerivedKey::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UGameObject* obj = Cast<UGameObject>( MeshComp->GetOwner()->FindComponentByClass<UGameObject>() );
	if( !obj ) return;

	obj->SetIsEnableDerivedKey( true );
}
