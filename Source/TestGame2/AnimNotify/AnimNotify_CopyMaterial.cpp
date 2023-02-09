// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterial.h"
#include "../Util/UtilMaterial.h"
#include "../Component/MaterialComp.h"

FString UAnimNotify_CopyMaterial::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterial::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	AActor* owner = Cast<AActor>( MeshComp->GetOwner() );
	if ( !owner )
		return;

	UMaterialComp* matProperty = Cast<UMaterialComp>( owner->FindComponentByClass<UMaterialComp>() );
	if( !matProperty ) 
		return;

	matProperty->SetMatState( UtilMaterial::GetSteppedMatrialInterface( owner ) );
}