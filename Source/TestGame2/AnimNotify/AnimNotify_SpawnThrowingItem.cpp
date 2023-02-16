// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnThrowingItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Component/ObjectComp.h"
#include "../Component/WeaponComp.h"

FString UAnimNotify_SpawnThrowingItem::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnThrowingItem::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if ( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UWeaponComp* weaponComp = Cast<UWeaponComp>( MeshComp->GetOwner()->FindComponentByClass<UWeaponComp>() );
	if ( !weaponComp )
		return;

	const auto& weaponInfo = GetDataInfoManager().GetWeaponInfos().Find( weaponComp->GetWeaponState() );
	if ( !weaponInfo )
		return;

	Actor = weaponInfo->ThorwingBP;

	Super::Notify( MeshComp, Animation );

	weaponComp->UnEquipWeapon();
}