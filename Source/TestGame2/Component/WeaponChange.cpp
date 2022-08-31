#pragma once

#include "WeaponChange.h"
#include "MaterialProperty.h"
#include "GameObject.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Manager/DataInfoManager.h"

UWeaponChange::UWeaponChange()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter                   = nullptr;
	CurWeaponMesh                     = nullptr;
	DissolveMaterial                  = nullptr;
	DissovleMaterialInstance          = nullptr;
	WeaponDurability                  = 0;
	WeaponDurabilityMax               = 0;

	WeaponState = EWeaponState::MAX;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> materialAsset( TEXT( "/Game/Shader/Dissolve_Inst.Dissolve_Inst" ) );
	if( materialAsset.Succeeded() ) DissolveMaterial = materialAsset.Object;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	_InitWeaponMesh();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if( DissolveAmount <= 0.1f )
	{ 
		_DissovleAnimEnd();
	}
	else
	{ 
		DissolveAmount = FMath::Lerp( DissolveAmount, 0.f, GetWorld()->GetDeltaSeconds() * 3.f );
		if( DissovleMaterialInstance )
			DissovleMaterialInstance->SetScalarParameterValue( FName( TEXT( "Amount" ) ), DissolveAmount );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ��ȯ �������� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UWeaponChange::CanWeaponChange( EWeaponState InWeaponState )
{
	auto matProperty = OwningCharacter ? Cast<UMaterialProperty>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "MatProperty" ) ) ) : nullptr;
	auto gameObject = OwningCharacter ? Cast<UGameObject>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;

	if( !gameObject || !matProperty )
		return false;

	if( WeaponState != EWeaponState::MAX )
		return false;

	if( gameObject->GetAnimState() != EAnimState::IDLE_RUN )
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
void UWeaponChange::SubWeaponDurability( int InValue )
{
	WeaponDurability -= 1;

	if( WeaponDurability <= 0 )
		UnEquipWeapon();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���⸦ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::EquipWeapon( EWeaponState InWeaponState, bool InChangeAnim )
{
	if( InWeaponState == EWeaponState::MAX )
		return;

	if( WeaponMeshes.IsEmpty() )
		return;

	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( InWeaponState );
	if( !curWeaponInfo )
		return;

	// MaterialProperty �� �ְ� ���� ���¶�� WeaponInfo�� �ʿ������� ��ŭ ������������ �Ҹ�ȴ�.
	auto matProperty = OwningCharacter ? Cast<UMaterialProperty>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "MatProperty" ) ) ) : nullptr;
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

	for( auto iter : WeaponMeshes )
		iter.Value->SetVisibility( false );

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
void UWeaponChange::UnEquipWeapon()
{
	CurWeaponMesh = nullptr;
	WeaponState = EWeaponState::MAX;

	for( auto iter : WeaponMeshes )
		iter.Value->SetVisibility( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� �޽� �ּҸ� �����س��´�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::_InitWeaponMesh()
{
	for( const auto& iter : GetDataInfoManager().GetWeaponInfos() )
	{
		auto staticMesh = Cast<UStaticMeshComponent>( OwningCharacter->GetDefaultSubobjectByName( iter.Value.ComponentName ) );
		WeaponMeshes.Add( iter.Key, staticMesh );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �ִϸ��̼��� ���� ó���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::_DissovleAnimEnd()
{
	if ( !CurWeaponMesh )
		return;

	if( CurWeaponMesh->GetMaterial(0) != DissovleMaterialInstance )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	DissolveAmount = 0.f;

	// ���� ��Ƽ������ ���� ĳ���� ��Ƽ����� ������ ��Ƽ����� �ٲ��ش�.
	CurWeaponMesh->SetMaterial( 0, curMesh->GetMaterial( 0 ) );
}