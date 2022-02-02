#pragma once

#include "MaterialProperty.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/BoxComponent.h"

UMaterialProperty::UMaterialProperty()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwningCharacter = nullptr;

	MatState = EMaterialState::MAX;
	MatStateOnTile = EMaterialState::MAX;

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
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 현재 올라가 있는 타일의 물질을 복사한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::CopyMaterial()
{
	if( MatStateOnTile == EMaterialState::MAX || MatStateOnTile == MatState )
		return;

	SetMatState( MatStateOnTile, true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 물질을 변경한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::SetMatState( EMaterialState InMatState, bool InChangeAnim )
{
	if( InMatState == EMaterialState::MAX )
		return;

	if( !OwningCharacter )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	MatState = InMatState;
	curMesh->SetMaterial( 0, Materials[ (uint8)MatState ] );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										     bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OtherActor )
		return;

	// 자기 자신 충돌은 무시한다.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter )
		return;

	auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
	if( !meshOnTile )
		return;
	
	MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief EMaterialState를 머터리얼 애셋 주소로 바꿔준다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialProperty::_InitMatAssets()
{
	for( uint8 matState = 0; matState < (uint8)EMaterialState::MAX; matState++ )
	{ 
		FString path = "";
		switch( (EMaterialState)matState )
		{
		case EMaterialState::JELLY:
			path = TEXT( "/Game/StarterContent/Materials/M_Metal_Copper.M_Metal_Copper" );
			break;
		case EMaterialState::GRASS:
			path = TEXT( "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" );
			break;
		case EMaterialState::ROCK:
			path = TEXT( "/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" );
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
//// @brief EMaterialState를 머터리얼 애셋 주소로 바꿔준다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
EMaterialState UMaterialProperty::_ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
{
	EMaterialState matState = EMaterialState::MAX;

	FString path = InMaterial->GetPathName();

	if( path == "/Game/StarterContent/Materials/M_Metal_Copper.M_Metal_Copper" )
		matState = EMaterialState::JELLY;
	else if( path =="/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" )
		matState = EMaterialState::GRASS;
	else if( path =="/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" )
		matState = EMaterialState::ROCK;

	return matState;
}