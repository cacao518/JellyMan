// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_WeaponChange.h"
#include "../Component/WeaponComp.h"

FString UAnimNotify_WeaponChange::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_WeaponChange::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UWeaponComp* weaponComp = Cast<UWeaponComp>( MeshComp->GetOwner()->FindComponentByClass<UWeaponComp>() );
	if( !weaponComp ) return;

	weaponComp->EquipWeapon( WeaponState );
}