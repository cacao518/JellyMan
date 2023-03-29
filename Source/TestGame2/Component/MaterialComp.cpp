#pragma once

#include "MaterialComp.h"
#include "ObjectComp.h"
#include "WeaponComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "WaterBodyComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
#include "NiagaraSystem.h"
#include "../Actor/CharacterPC.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/ObjectManager.h"
#include "../System/MyAnimInstance.h"
#include "../Util/UtilMaterial.h"

UMaterialComp::UMaterialComp()
	:
InitMaterial    ( nullptr ),
OwningCharacter ( nullptr )
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;	
}

UMaterialComp::~UMaterialComp()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::BeginPlay()
{
	Super::BeginPlay();

	_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_ProcessGrass( DeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::SetMatState( UMaterialInterface* InMatInterface, bool InIsInit )
{
	if( InIsInit )
		InMatInterface = InitMaterial;

	if ( !InMatInterface )
		return;

	EMaterialState matState = UtilMaterial::ConvertMatAssetToMatState( InMatInterface );
	if( matState == EMaterialState::DEFAULT || matState == MatState )
		return;

	if( !OwningCharacter )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	curMesh->SetMaterial( 0, InMatInterface );

	auto weaponComp = OwningCharacter ? Cast<UWeaponComp>( OwningCharacter->FindComponentByClass<UWeaponComp>() ) : nullptr;
	if( weaponComp && weaponComp->GetCurWeaponMesh() )
		weaponComp->GetCurWeaponMesh()->SetMaterial( 0, InMatInterface );
	
	MatState = matState;

	_InitStatus();

	if ( !InIsInit )
	{
		_PlayChangeEffect();

	/*	FString str = OwningCharacter->GetName() + TEXT( ": Material Change -> " ) + InMatInterface->GetName();
		if ( GEngine )
			GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );*/
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
float UMaterialComp::GetIntensity()
{
	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( matInfo )
	{
		return matInfo->Intensity;
	}

	return 1.f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �߰� ���� ���� ���θ� ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMaterialComp::IsMiddleIntensity()
{
	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( matInfo )
	{
		if( matInfo->Intensity >= Const::MIDDLE_RATE )
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �� ���� ���� ���θ� ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMaterialComp::IsHardIntensity()
{
	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( matInfo )
	{
		if( matInfo->Intensity >= Const::HARD_RATE )
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  �ʱ�ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_Init()
{
	MatState = EMaterialState::DEFAULT;
	
	OwningCharacter = Cast<ACharacter>( GetOwner() );

	SetMatState( nullptr, true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  ��Ƽ���� �´� �ɷ�ġ�� �ʱ�ȭ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_InitStatus()
{
	auto objectComp = OwningCharacter ? Cast<UObjectComp>( OwningCharacter->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !objectComp )
		return;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( !characterMovement )
		return;

	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( matInfo )
	{
		objectComp->SetMoveSpeed   ( ( Const::MAX_MASS - matInfo->Mass ) * objectComp->GetInitStat().MoveSpeed );
		objectComp->SetAttackSpeed ( ( Const::MAX_MASS - matInfo->Mass ) * objectComp->GetInitStat().AttackSpeed );
		objectComp->SetJumpPower   ( ( Const::MAX_MASS - matInfo->Mass ) * objectComp->GetInitStat().JumpPower );
		objectComp->SetAttackPower ( matInfo->Intensity              * objectComp->GetInitStat().AttackPower );
		objectComp->SetDefensePower( matInfo->Intensity              * objectComp->GetInitStat().DefensePower );
		objectComp->SetStrength    ( matInfo->Intensity              * objectComp->GetInitStat().Strength  );
		objectComp->SetWeight      ( matInfo->Mass                                                         );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( matInfo->CollisonName );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ���� ����Ʈ�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_PlayChangeEffect()
{
	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( !matInfo )
		return;

	GetObjectManager().SpawnParticle( matInfo->ChangeParticleName, OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Ǯ���� ���� ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_ProcessGrass( float InDeltaTime )
{
	if( MatState != EMaterialState::GRASS )
		return;

	UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( OwningCharacter->GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	auto charMovement = OwningCharacter->GetCharacterMovement();
	if( !charMovement )
		return;

	if( animInstance->IsFly )
	{
		if( charMovement->Velocity.Z > 0 )
			charMovement->Velocity.Z = 0;

		charMovement->GravityScale = Const::FLY_GRAVITY_SCALE;
	}
	else
		charMovement->GravityScale = Const::DEFAULT_GRAVITY_SCALE;
}