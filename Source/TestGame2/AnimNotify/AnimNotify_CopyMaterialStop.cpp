// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterialStop.h"
#include "../Component/MaterialProperty.h"

FString UAnimNotify_CopyMaterialStop::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterialStop::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UMaterialProperty* matProperty = Cast<UMaterialProperty>( MeshComp->GetOwner()->FindComponentByClass<UMaterialProperty>() );
	if( !matProperty ) return;

	matProperty->SetIsEnabledTileColl( false );
}