// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterial.h"
#include "../Component/MaterialComp.h"

FString UAnimNotify_CopyMaterial::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterial::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UMaterialComp* matProperty = Cast<UMaterialComp>( MeshComp->GetOwner()->FindComponentByClass<UMaterialComp>() );
	if( !matProperty ) return;

	matProperty->SetIsEnabledTileColl( true );
}