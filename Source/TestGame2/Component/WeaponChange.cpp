#pragma once

#include "WeaponChange.h"
#include "MaterialProperty.h"
#include "GameObject.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
//// @brief 무기를 변경한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::SetWeaponState( EWeaponState InWeaponState, bool InChangeAnim )
{
	if( InWeaponState == EWeaponState::MAX )
		return;

	if( WeaponMeshes.IsEmpty() )
		return;

	WeaponState = InWeaponState;
	CurWeaponMesh = WeaponMeshes[ (uint8)WeaponState ];

	if( !CurWeaponMesh )
		return;

	for( auto mesh : WeaponMeshes )
		mesh->SetVisibility( false );

	CurWeaponMesh->SetVisibility( true );

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

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
//// @brief 무기 메쉬 주소를 저장해놓는다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponChange::_InitWeaponMesh()
{
	for( uint8 i = 0; i<(uint8)EWeaponState::MAX; i++ )
	{
		switch( (EWeaponState)i )
		{
		case EWeaponState::SWORD:
			auto staticMesh = Cast<UStaticMeshComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "Sword" ) ) );
			WeaponMeshes.Push( staticMesh );
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 디졸브 애니메이션을 종료 처리한다.
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

	// 무기 머티리얼을 현재 캐릭터 머티리얼과 동일한 머티리얼로 바꿔준다.
	CurWeaponMesh->SetMaterial( 0, curMesh->GetMaterial( 0 ) );
}