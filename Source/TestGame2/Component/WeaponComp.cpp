#pragma once

#include "WeaponComp.h"
#include "MaterialComp.h"
#include "CharacterComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Manager/DataInfoManager.h"

UWeaponComp::UWeaponComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter                   = nullptr;
	CurWeaponMesh                     = nullptr;
	DissolveMaterial                  = nullptr;
	DissovleMaterialInstance          = nullptr;
	WeaponDurability                  = 0;
	WeaponDurabilityMax               = 0;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> materialAsset( TEXT( "/Game/Shader/Dissolve_Inst.Dissolve_Inst" ) );
	if( materialAsset.Succeeded() ) DissolveMaterial = materialAsset.Object;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	_InitWeaponMesh();
	EquipWeapon( WeaponState, false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if( DissolveAmount <= 0.1f )
	{ 
		_DissovleAnimEnd();
	}
	else
	{ 
		DissolveAmount = FMath::Lerp( DissolveAmount, 0.f, GetWorld()->GetDeltaSeconds() * 5.f );
		if( DissovleMaterialInstance )
			DissovleMaterialInstance->SetScalarParameterValue( FName( TEXT( "Amount" ) ), DissolveAmount );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ��ȯ �������� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UWeaponComp::CanWeaponComp( EWeaponState InWeaponState )
{
	auto matProperty = OwningCharacter ? Cast<UMaterialComp>( OwningCharacter->FindComponentByClass<UMaterialComp>() ) : nullptr;
	auto characterComp = OwningCharacter ? Cast<UCharacterComp>( OwningCharacter->FindComponentByClass<UCharacterComp>() ) : nullptr;

	if( !characterComp || !matProperty )
		return false;

	if( WeaponState != EWeaponState::DEFAULT )
		return false;

	if( characterComp->GetAnimState() != EAnimState::IDLE_RUN )
		return false;

	if( matProperty->GetMatState() != EMaterialState::JELLY )
		return false;

	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( InWeaponState );
	if( !curWeaponInfo )
		return false;

	if( matProperty->GetJellyEnergy() >= curWeaponInfo->RequireJellyAmount )
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� �������� ���ҽ�Ų��.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::SubWeaponDurability()
{
	WeaponDurability -= 1;

	if( WeaponDurability <= 0 )
		UnEquipWeapon();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���⸦ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::EquipWeapon( EWeaponState InWeaponState, bool InChangeAnim )
{
	if( InWeaponState == EWeaponState::DEFAULT )
		return;

	if( WeaponMeshes.IsEmpty() )
		return;

	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( InWeaponState );
	if( !curWeaponInfo )
		return;

	// MaterialComp �� �ְ� ���� ���¶�� WeaponInfo�� �ʿ������� ��ŭ ������������ �Ҹ�ȴ�.
	auto matProperty = OwningCharacter ? Cast<UMaterialComp>( OwningCharacter->FindComponentByClass<UMaterialComp>() ) : nullptr;
	if( matProperty && matProperty->GetMatState() == EMaterialState::JELLY )
	{
		if( matProperty->GetJellyEnergy() >= curWeaponInfo->RequireJellyAmount )
			matProperty->SetJellyEnergy( matProperty->GetJellyEnergy() - curWeaponInfo->RequireJellyAmount );
		else
			return;
	}

	WeaponState = InWeaponState;
	CurWeaponMesh = WeaponMeshes.FindRef( WeaponState );
	WeaponDurability = curWeaponInfo->DurabilityMax;
	WeaponDurabilityMax = curWeaponInfo->DurabilityMax;

	if( !CurWeaponMesh )
		return;

	for( auto& [_, mesh] : WeaponMeshes )
		mesh->SetVisibility( false );

	CurWeaponMesh->SetVisibility( true );

	if( InChangeAnim )
	{ 
		DissolveAmount = 1.f;
		DissovleMaterialInstance = UMaterialInstanceDynamic::Create( DissolveMaterial, this );
		CurWeaponMesh->SetMaterial( 0, DissovleMaterialInstance );
	}
	else
	{
		_DissovleAnimEnd();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���⸦ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::UnEquipWeapon()
{
	CurWeaponMesh = nullptr;
	WeaponState = EWeaponState::DEFAULT;
	WeaponDurability = 0;

	for( auto& [_, mesh] : WeaponMeshes )
		mesh->SetVisibility( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� �޽� �ּҸ� �����س��´�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_InitWeaponMesh()
{
	for( const auto& [state, weaponInfo] : GetDataInfoManager().GetWeaponInfos() )
	{
		auto staticMesh = Cast<UStaticMeshComponent>( OwningCharacter->GetDefaultSubobjectByName( weaponInfo.ComponentName ) );
		WeaponMeshes.Add( state, staticMesh );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �ִϸ��̼��� ���� ó���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_DissovleAnimEnd()
{
	if ( !CurWeaponMesh )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	DissolveAmount = 0.f;

	// ���� ��Ƽ������ ���� ĳ���� ��Ƽ����� ������ ��Ƽ����� �ٲ��ش�.
	CurWeaponMesh->SetMaterial( 0, curMesh->GetMaterial( 0 ) );
}