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
//// @brief ���⸦ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::SetWeaponState( EWeaponState InWeaponState, bool InChangeAnim )
{
	if( InWeaponState == EWeaponState::MAX )
		return;

	if( WeaponMeshes.IsEmpty() )
		return;

	WeaponState = InWeaponState;
	CurWeaponMesh = WeaponMeshes.FindRef( WeaponState );

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
//// @brief ���� �޽� �ּҸ� �����س��´�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::_InitWeaponMesh()
{
	for( const auto& iter : GetDataInfoManager().GetWeaponInfos() )
	{
		auto staticMesh = Cast<UStaticMeshComponent>( OwningCharacter->GetDefaultSubobjectByName( iter.second.ComponentName ) );
		WeaponMeshes.Add( iter.first, staticMesh );
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