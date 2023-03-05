// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnThrowingItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "../Actor/Projectile.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/LockOnManager.h"
#include "../Component/ObjectComp.h"
#include "../Component/WeaponComp.h"
#include "UObject/ConstructorHelpers.h"

FString UAnimNotify_SpawnThrowingItem::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnThrowingItem::SetProperty( AActor* InOwner )
{
	if ( !InOwner )
		return;

	/* Set Actor */
	UWeaponComp* weaponComp = Cast<UWeaponComp>( InOwner->FindComponentByClass<UWeaponComp>() );
	if ( !weaponComp )
		return;

	const auto& weaponInfo = GetDataInfoManager().GetWeaponInfos().Find( weaponComp->GetCurWeaponNum() );
	if ( !weaponInfo )
		return;

	FString path = weaponInfo->ThorwingBPPath;
	UClass* thorwingBP = ConstructorHelpersInternal::FindOrLoadClass( path, AProjectile::StaticClass() );
	ResultActor = thorwingBP;

	/* Set Pos */
	UObjectComp* objComp = Cast<UObjectComp>( InOwner->FindComponentByClass<UObjectComp>() );
	if ( !objComp )
		return;

	auto spawnPosComp = Cast<USceneComponent>( InOwner->GetDefaultSubobjectByName( TEXT( "SpawnPosComp" ) ) );
	if ( !spawnPosComp )
		return;

	FVector relativeSpawnPos = spawnPosComp->GetRelativeLocation() + Pos;
	FVector worldSpawnPos = UKismetMathLibrary::TransformLocation( spawnPosComp->GetComponentTransform(), relativeSpawnPos );

	ResultPos = worldSpawnPos;

	/* Set Rotate */
	ACharacter* lockOnTarget = GetLockOnManager().GetLockOnTarget();
	if ( lockOnTarget )
		ResultRotate = UKismetMathLibrary::FindLookAtRotation( InOwner->GetActorLocation(), lockOnTarget->GetActorLocation() );
	else
		ResultRotate = InOwner->GetActorRotation() + Rotate;
}

void UAnimNotify_SpawnThrowingItem::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if ( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	AActor* owner = Cast<AActor>( MeshComp->GetOwner() );
	if ( !owner )
		return;

	UWeaponComp* weaponComp = Cast<UWeaponComp>( owner->FindComponentByClass<UWeaponComp>() );
	if ( !weaponComp )
		return;

	Super::Notify( MeshComp, Animation );

	weaponComp->UnEquipWeapon();
}