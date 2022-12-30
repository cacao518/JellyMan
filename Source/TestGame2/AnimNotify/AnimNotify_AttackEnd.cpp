// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"
#include "../Component/ObjectComp.h"

FString UAnimNotify_AttackEnd::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_AttackEnd::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UObjectComp* obj = Cast<UObjectComp>( MeshComp->GetOwner()->FindComponentByClass<UObjectComp>() );
	if( !obj ) return;

	obj->SetIsEnabledAttackColl( false );
}
