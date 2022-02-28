// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_WeaponChange.h"
#include "../Component/WeaponChange.h"

FString UAnimNotify_WeaponChange::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_WeaponChange::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UWeaponChange* weaponChange = Cast<UWeaponChange>( MeshComp->GetOwner()->FindComponentByClass<UWeaponChange>() );
	if( !weaponChange ) return;

	weaponChange->SetWeaponState( WeaponState );
}