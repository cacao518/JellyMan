// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DerivedKey.h"
#include "../Component/CharacterComp.h"

FString UAnimNotify_DerivedKey::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DerivedKey::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UCharacterComp* obj = Cast<UCharacterComp>( MeshComp->GetOwner()->FindComponentByClass<UCharacterComp>() );
	if( !obj ) return;

	obj->SetIsEnableDerivedKey( true );
}
