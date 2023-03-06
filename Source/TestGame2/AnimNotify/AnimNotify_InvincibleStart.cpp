// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_InvincibleStart.h"
#include "../Component/ObjectComp.h"

FString UAnimNotify_InvincibleStart::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_InvincibleStart::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UObjectComp* obj = Cast<UObjectComp>( MeshComp->GetOwner()->FindComponentByClass<UObjectComp>() );
	if( !obj ) 
		return;

	obj->SetIsEnabledHitColl( false );
}
