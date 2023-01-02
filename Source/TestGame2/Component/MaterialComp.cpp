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
#include "../Character/CharacterPC.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/ObjectManager.h"
#include "../System/MyAnimInstance.h"

UMaterialComp::UMaterialComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;	
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

	_ProcessGauge( DeltaTime );
	_ProcessGrass( DeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::SetMatState( UMaterialInterface* InMatInterface )
{
	if( !InitMaterial )
		return;

	// ��ȭ�� ������ ������ �ʱ� ���� ���·� ���ư���.
	if( !InMatInterface )
		InMatInterface = InitMaterial;

	EMaterialState matState = _ConvertMatAssetToMatState( InMatInterface );

	if( matState == EMaterialState::MAX || matState == MatState )
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

	SetIsEnabledTileColl( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Ÿ�� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::SetIsEnabledTileColl( bool InIsEnabled )
{
	IsEnabledTileColl = InIsEnabled;

	auto tileColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "TileColl" ) ) ) : nullptr;
	if( tileColl )
	{
		tileColl->SetCollisionEnabled( IsEnabledTileColl ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision );
		tileColl->SetVisibility( IsEnabledTileColl );
	}
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
//// @brief �� �߷� ���� ���θ� ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMaterialComp::IsHeavyMass()
{
	const auto& matInfo = GetDataInfoManager().GetMaterialInfos().Find( MatState );
	if( matInfo )
	{
		if( matInfo->Mass >= Const::HARD_RATE )
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										     bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OtherActor )
		return;

	// �ڱ� �ڽ� �浹�� �����Ѵ�.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter )
		return;

	_ProcessCollision( OtherActor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  �ʱ�ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_Init()
{
	MatState = EMaterialState::MAX;

	JellyEnergy = 0.f;
	JellyEnergyMax = 0.f;
	MatEnergy = 0.f;
	MatEnergyMax = 0.f;

	const auto& jellyMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::JELLY );
	if( jellyMatInfo )
	{
		JellyEnergy    = jellyMatInfo->MatEnergyMax;
		JellyEnergyMax = jellyMatInfo->MatEnergyMax;
	}

	const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::WATER );
	if( waterMatInfo )
		WaterMaterial = LoadObject<UMaterialInterface>( NULL, *( waterMatInfo->AssetPath ), NULL, LOAD_None, NULL );
	
	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto tileColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "TileColl" ) ) ) : nullptr;
	if( tileColl )
		tileColl->OnComponentBeginOverlap.AddDynamic( this, &UMaterialComp::TileCollBeginOverlap );

	SetIsEnabledTileColl( false );

	SetMatState();
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
		objectComp->SetWeight      ( matInfo->Mass                   * objectComp->GetInitStat().Weight );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( matInfo->CollisonName );

		MatEnergy    = matInfo->MatEnergyMax;
		MatEnergyMax = matInfo->MatEnergyMax;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief EMaterialState�� ���͸��� �ּ� �ּҷ� �ٲ��ش�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
EMaterialState UMaterialComp::_ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
{
	FString path = InMaterial->GetPathName();

	for( const auto& [state, matInfo] : GetDataInfoManager().GetMaterialInfos() )
	{
		if( path == matInfo.AssetPath )
			return state;
	}

	return EMaterialState::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �浹 ó���� �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_ProcessCollision( AActor* InOtherActor )
{
	UMaterialInterface* matInterface = nullptr;

	// ���ͳ� �÷��̾�
	if( Cast<ACharacter>( InOtherActor ) )
	{
		/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
		if( !meshOnTile )
			return;

		MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
	}
	else
	{
		// ����ƽ �Ž�
		auto staticMesh = Cast<UStaticMeshComponent>( InOtherActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
		if( staticMesh )
		{
			matInterface = staticMesh->GetMaterial( 0 );
			if( !matInterface )
				return;
		}

		// ���� �ٵ�
		auto waterBody = Cast<UWaterBodyComponent>( InOtherActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
		if( waterBody )
		{
			matInterface = WaterMaterial;
		}

		// ���彺������
		auto landScape = Cast<ULandscapeComponent>( InOtherActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
		if( landScape )
		{
			auto proxy = landScape->GetLandscapeProxy();
			if( !proxy )
			{
				if( GEngine )
					GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, "proxy is null" );
				return;
			}

			matInterface = proxy->GetLandscapeMaterial( 0 );
			if( !matInterface )
				return;
		}
	}

	FString str = OwningCharacter->GetName() + TEXT( ": Material Change -> " ) + matInterface->GetName();
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );

	SetIsEnabledTileColl( false );
	SetMatState( matInterface );
	_PlayChangeEffect();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ���� ����Ʈ�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_PlayChangeEffect()
{
	switch( MatState )
	{
		case EMaterialState::GRASS:
		{
			GetObjectManager().SpawnParticle( TEXT( "GrassChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
		case EMaterialState::ROCK:
		{
			GetObjectManager().SpawnParticle( TEXT( "RockChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
		case EMaterialState::GRAVEL:
		{
			GetObjectManager().SpawnParticle( TEXT( "GravelChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ���� ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_ProcessGauge( float InDeltaTime )
{
	if( MatState == EMaterialState::JELLY )
		JellyEnergy < JellyEnergyMax ? JellyEnergy += InDeltaTime * 2.f : JellyEnergy = JellyEnergyMax;
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