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
#include "../Util/UtilMaterial.h"

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
//// @brief 물질을 변경한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::SetMatState( UMaterialInterface* InMatInterface )
{
	if( !InitMaterial )
		return;

	// 변화할 물질이 없으면 초기 물질 상태로 돌아간다.
	if( !InMatInterface )
		InMatInterface = InitMaterial;

	EMaterialState matState = UtilMaterial::ConvertMatAssetToMatState( InMatInterface );

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
//// @brief 타일 콜리전 활성화 여부를 셋팅한다.
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
//// @brief 중간 강도 인지 여부를 반환한다.
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
//// @brief 고 강도 인지 여부를 반환한다.
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
//// @brief 고 중량 인지 여부를 반환한다.
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
//// @brief 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										     bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OtherActor )
		return;

	// 자기 자신 충돌은 무시한다.
	if( Cast<ACharacter>( OtherActor ) == OwningCharacter )
		return;

	_ProcessCollision( OtherActor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  초기화 한다.
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
	
	OwningCharacter = Cast<ACharacter>( GetOwner() );

	auto tileColl = OwningCharacter ? Cast<UBoxComponent>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "TileColl" ) ) ) : nullptr;
	if( tileColl )
		tileColl->OnComponentBeginOverlap.AddDynamic( this, &UMaterialComp::TileCollBeginOverlap );

	SetIsEnabledTileColl( false );

	SetMatState();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief  머티리얼에 맞는 능력치를 초기화한다.
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
//// @brief 충돌 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_ProcessCollision( AActor* InOtherActor )
{
	UMaterialInterface* matInterface = UtilMaterial::GetMatrialInterface( InOtherActor );

	FString str = OwningCharacter->GetName() + TEXT( ": Material Change -> " ) + matInterface->GetName();
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, str );

	SetIsEnabledTileColl( false );
	SetMatState( matInterface );
	_PlayChangeEffect();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 물질 변경 이펙트를 실행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_PlayChangeEffect()
{
	switch( MatState )
	{
		case EMaterialState::GRASS:
		{
			GetObjectManager().SpawnParticle( TEXT( "MaterialChange/GrassChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
		case EMaterialState::ROCK:
		{
			GetObjectManager().SpawnParticle( TEXT( "MaterialChange/RockChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
		case EMaterialState::GRAVEL:
		{
			GetObjectManager().SpawnParticle( TEXT( "MaterialChange/GravelChange" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 게이지 관련 로직을 실행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMaterialComp::_ProcessGauge( float InDeltaTime )
{
	if( MatState == EMaterialState::JELLY )
		JellyEnergy < JellyEnergyMax ? JellyEnergy += InDeltaTime * 2.f : JellyEnergy = JellyEnergyMax;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 풀재질 관련 로직을 실행한다.
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