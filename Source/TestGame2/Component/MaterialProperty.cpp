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

UMaterialProperty::UMaterialProperty()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;

	MatState = EMaterialState::MAX;

	_InitMatAssets();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto tileColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "TileColl" ) ) ) : nullptr;
	if( tileColl )
		tileColl->OnComponentBeginOverlap.AddDynamic( this, &UMaterialProperty::TileCollBeginOverlap );

	SetIsEnabledTileColl( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_ProcessHeavyMaterial();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::SetMatState( EMaterialState InMatState, bool InChangeAnim )
{
	if( InMatState == EMaterialState::MAX || InMatState == MatState )
		return;

	if( !OwningCharacter )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	MatState = InMatState;
	curMesh->SetMaterial( 0, Materials[ (uint8)MatState ] );

	auto weaponChange = OwningCharacter ? Cast<UWeaponChange>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "WeaponChange" ) ) ) : nullptr;
	if( weaponChange && weaponChange->GetCurWeaponMesh() )
		weaponChange->GetCurWeaponMesh()->SetMaterial( 0, Materials[ (uint8)MatState ] );
	
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

	EMaterialState matStateOnTile = EMaterialState::MAX;

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
			auto material = staticMesh->GetMaterial( 0 );
			if( !material )
				return;

			matStateOnTile = _ConvertMatAssetToMatState( material );

			FString str = OwningCharacter->GetName()+TEXT( ": Material Change -> " )+material->GetName();
			if( GEngine )
				GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );
		}

		// ���彺������
		auto landScape = Cast<ULandscapeComponent>( OtherActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
		if( landScape )
		{
			auto material = landScape->GetMaterial( 0 );
			if( !material )
				return;

			matStateOnTile = _ConvertMatAssetToMatState( material );

			FString str = OwningCharacter->GetName()+TEXT( ": Material Change -> " )+material->GetName();
			if( GEngine )
				GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );
		}
	}

	SetMatState( matStateOnTile, true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  ���͸��� �ּ� �ּҸ� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_InitMatAssets()
{
	for( uint8 matState = 0; matState < (uint8)EMaterialState::MAX; matState++ )
	{ 
		FString path = "";
		switch( (EMaterialState)matState )
		{
		case EMaterialState::JELLY:
			path = TEXT( "/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor" );
			break;
		case EMaterialState::GRASS:
			path = TEXT( "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" );
			break;
		case EMaterialState::ROCK:
			path = TEXT( "/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" );
			break;
		case EMaterialState::GRAVEL:
			path = TEXT( "/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel" );
			break;
		case EMaterialState::WATER:
			path = TEXT( "/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake" );
			break;
		case EMaterialState::MAX:
			break;
		default:
			break;
		}

		ConstructorHelpers::FObjectFinder<UMaterialInterface> materialAsset( *path );
		if( materialAsset.Succeeded() )
		{
			Materials.Push( materialAsset.Object );
		}
	}
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

	switch( MatState )
	{
	case EMaterialState::JELLY:
	{
		gameObject->SetMoveSpeed( 1.f );
		gameObject->SetAttackSpeed( 1.f );
		gameObject->SetJumpPower( 1.f );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( "Pawn" );
		break;
	}
	case EMaterialState::GRASS:
	{
		gameObject->SetMoveSpeed( Const::GRASS_MOVE_SPEED );
		gameObject->SetAttackSpeed( Const::GRASS_ATTACK_SPEED );
		gameObject->SetJumpPower( Const::GRASS_JUMP_POWER );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( "Pawn" );
		break;
	}
	case EMaterialState::ROCK:
	{
		gameObject->SetMoveSpeed( Const::ROCK_MOVE_SPEED );
		gameObject->SetAttackSpeed( Const::ROCK_ATTACK_SPEED );
		gameObject->SetJumpPower( Const::ROCK_JUMP_POWER );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( "Pawn" );
		break;
	}
	case EMaterialState::GRAVEL:
	{
		gameObject->SetMoveSpeed( Const::GRAVEL_MOVE_SPEED );
		gameObject->SetAttackSpeed( Const::GRAVEL_ATTACK_SPEED );
		gameObject->SetJumpPower( Const::GRAVEL_JUMP_POWER );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( "Pawn" );
		break;
	}
	case EMaterialState::WATER:
	{
		gameObject->SetMoveSpeed( Const::WATER_MOVE_SPEED );
		gameObject->SetAttackSpeed( Const::WATER_ATTACK_SPEED );
		gameObject->SetJumpPower( Const::WATER_JUMP_POWER );
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( "Water" );
		break;
	}
	case EMaterialState::MAX:
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief EMaterialState�� ���͸��� �ּ� �ּҷ� �ٲ��ش�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
EMaterialState UMaterialProperty::_ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
{
	EMaterialState matState = EMaterialState::MAX;

	FString path = InMaterial->GetPathName();

	if( path == "/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor" )
		matState = EMaterialState::JELLY;
	else if( path =="/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" )
		matState = EMaterialState::GRASS;
	else if( path =="/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" )
		matState = EMaterialState::ROCK;
	else if( path=="/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel" )
		matState = EMaterialState::GRAVEL;
	else if( path=="/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake" )
		matState = EMaterialState::WATER;

	return matState;
}

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