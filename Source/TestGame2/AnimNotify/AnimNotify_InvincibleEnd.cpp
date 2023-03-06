// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_InvincibleEnd.h"
#include "../Component/ObjectComp.h"

FString UAnimNotify_InvincibleEnd::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_InvincibleEnd::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UObjectComp* obj = Cast<UObjectComp>( MeshComp->GetOwner()->FindComponentByClass<UObjectComp>() );
	if( !obj )
		return;

	obj->SetIsEnabledHitColl( true );
}
