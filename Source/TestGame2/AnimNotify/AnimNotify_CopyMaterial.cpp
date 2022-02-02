// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterial.h"
#include "../Component/MaterialProperty.h"

FString UAnimNotify_CopyMaterial::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterial::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UMaterialProperty* matProperty = Cast<UMaterialProperty>( MeshComp->GetOwner()->FindComponentByClass<UMaterialProperty>() );
	if( !matProperty ) return;

	matProperty->CopyMaterial();
}