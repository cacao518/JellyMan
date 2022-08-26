#pragma once

#include "MaterialProperty.h"
#include "GameObject.h"
#include "WeaponChange.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
#include "../Manager/DataInfoManager.h"

UMaterialProperty::UMaterialProperty()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::BeginPlay()
{
	Super::BeginPlay();

	_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_ProcessGauge( DeltaTime );
	_ProcessHeavyMaterial();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::SetMatState( UMaterialInterface* InMatInterface )
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

	auto weaponChange = OwningCharacter ? Cast<UWeaponChange>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "WeaponChange" ) ) ) : nullptr;
	if( weaponChange && weaponChange->GetCurWeaponMesh() )
		weaponChange->GetCurWeaponMesh()->SetMaterial( 0, InMatInterface );
	
	MatState = matState;

	_InitStatus();

	SetIsEnabledTileColl( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Ÿ�� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::SetIsEnabledTileColl( bool InIsEnabled )
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
//// @brief �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										     bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OtherActor )
		return;

	// �ڱ� �ڽ� �浹�� �����Ѵ�.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter )
		return;

	UMaterialInterface* matInterface = nullptr;

	// ���ͳ� �÷��̾�
	if( Cast<ACharacter>( OtherActor ) )
	{ 
		/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
		if( !meshOnTile )
			return;

		MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
	}
	else
	{
		// ����ƽ �Ž�
		auto staticMesh = Cast<UStaticMeshComponent>( OtherActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
		if( staticMesh )
		{
			matInterface = staticMesh->GetMaterial( 0 );
			if( !matInterface )
				return;

			FString str = OwningCharacter->GetName()+TEXT( ": Material Change -> " )+ matInterface->GetName();
			if( GEngine )
				GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );
		}

		// ���彺������
		auto landScape = Cast<ULandscapeComponent>( OtherActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
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

			FString str = OwningCharacter->GetName()+TEXT( ": Material Change -> " )+ matInterface->GetName();
			if( GEngine )
				GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );
		}
	}

	SetMatState( matInterface );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  �ʱ�ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_Init()
{
	MatState = EMaterialState::MAX;

	JellyGauge = 0.f;
	JellyGaugeMax = 0.f;
	MatGauge = 0.f;
	MatGaugeMax = 0.f;

	auto iter = GetDataInfoManager().GetMaterialInfos().find( EMaterialState::JELLY );
	if( iter != GetDataInfoManager().GetMaterialInfos().end() )
		JellyGaugeMax = ( *iter ).second.MatGaugeMax;
	
	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto tileColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "TileColl" ) ) ) : nullptr;
	if( tileColl )
		tileColl->OnComponentBeginOverlap.AddDynamic( this, &UMaterialProperty::TileCollBeginOverlap );

	SetIsEnabledTileColl( false );

	SetMatState();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  ��Ƽ���� �´� �ɷ�ġ�� �ʱ�ȭ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_InitStatus()
{
	auto gameObject = OwningCharacter ? Cast<UGameObject>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( !characterMovement )
		return;

	for( const auto pair : GetDataInfoManager().GetMaterialInfos() )
	{
		if( MatState == pair.first )
		{ 
			const MaterialInfo matInfo = pair.second;
			gameObject->SetMoveSpeed   ( matInfo.MoveSpeed   * gameObject->GetInitStat().MoveSpeed );
			gameObject->SetAttackSpeed ( matInfo.AttackSpeed * gameObject->GetInitStat().AttackSpeed );
			gameObject->SetJumpPower   ( matInfo.JumpPower   * gameObject->GetInitStat().JumpPower );
			gameObject->SetAttackPower ( matInfo.Mass        * gameObject->GetInitStat().AttackPower );
			gameObject->SetDefensePower( matInfo.Mass        * gameObject->GetInitStat().DefensePower );
			OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( matInfo.CollisonName );

			MatGauge    = matInfo.MatGaugeMax;
			MatGaugeMax = matInfo.MatGaugeMax;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief EMaterialState�� ���͸��� �ּ� �ּҷ� �ٲ��ش�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
EMaterialState UMaterialProperty::_ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
{
	FString path = InMaterial->GetPathName();

	for( const auto pair : GetDataInfoManager().GetMaterialInfos() )
	{
		const MaterialInfo matInfo = pair.second;
		if( path == matInfo.AssetPath )
			return pair.first;
	}

	return EMaterialState::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ���� ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_ProcessGauge( float InDeltaTime )
{
	if( MatState == EMaterialState::JELLY )
		JellyGauge < JellyGaugeMax ? JellyGauge += InDeltaTime * 2.f : JellyGauge = JellyGaugeMax;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���ſ� ���� ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_ProcessHeavyMaterial()
{
	auto moveComponent = OwningCharacter->GetMovementComponent();
	if( moveComponent )
	{
		if( moveComponent->IsFalling() )
		{
			FallingShakeToWeightOnce = false;
		}
		else if( !FallingShakeToWeightOnce )
		{
			auto gameObject = OwningCharacter ? Cast<UGameObject>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
			if( gameObject )
			{
				gameObject->CameraShake( 1.f, true );
				FallingShakeToWeightOnce = true;
			}
		}
	}
}