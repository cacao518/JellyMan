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

UWeaponComp::~UWeaponComp()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>( GetOwner() );

	RegisteredWeaponNums.Add( EWeaponState::SWORD, 1 );
	RegisteredWeaponNums.Add( EWeaponState::AXE,   2 );
	RegisteredWeaponNums.Add( EWeaponState::SPEAR, 3 );

	_InitWeaponMesh();
	EquipWeapon( WeaponState, false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_CoolingWeapons( DeltaTime );

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
//// @brief 무기 소환 가능한지 여부
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

	int weaponNum = RegisteredWeaponNums.FindRef( InWeaponState );
	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( weaponNum );
	if( !curWeaponInfo )
		return false;

	float* cooltime = CoolingWeapons.Find( InWeaponState );
	if( cooltime )
	{
		if( *cooltime > 0 )
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 무기 내구도를 감소시킨다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::SubWeaponDurability()
{
	WeaponDurability -= 1;

	if( WeaponDurability <= 0 )
		UnEquipWeapon();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 무기를 변경한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::EquipWeapon( EWeaponState InWeaponState, bool InChangeAnim )
{
	if( InWeaponState == EWeaponState::DEFAULT )
		return;

	if( WeaponMeshes.IsEmpty() || RegisteredWeaponNums.IsEmpty() )
		return;

	int weaponNum = RegisteredWeaponNums.FindRef( InWeaponState );
	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( weaponNum );
	if( !curWeaponInfo )
		return;

	WeaponState = curWeaponInfo->State;
	CurWeaponNum = weaponNum;
	CurWeaponMesh = WeaponMeshes.FindRef( weaponNum );
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
//// @brief 무기를 해제한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::UnEquipWeapon()
{
	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( CurWeaponNum );
	if( !curWeaponInfo )
		return;

	_RegisterCoolTime( *curWeaponInfo );

	CurWeaponNum = 0;
	CurWeaponMesh = nullptr;
	WeaponState = EWeaponState::DEFAULT;
	WeaponDurability = 0;

	for( auto& [_, mesh] : WeaponMeshes )
		mesh->SetVisibility( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 해당 무기 쿨타임 퍼센트를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
float UWeaponComp::GetWeaponCoolTimePercent( EWeaponState InWeaponState )
{
	auto remainCoolTime = CoolingWeapons.Find( InWeaponState );
	if( !remainCoolTime )
		return 0;

	int weaponNum = RegisteredWeaponNums.FindRef( InWeaponState );
	const auto& curWeaponInfo = GetDataInfoManager().GetWeaponInfos().Find( weaponNum );
	if( !curWeaponInfo )
		return 0;

	return *remainCoolTime / curWeaponInfo->CoolTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 무기 메쉬 주소를 저장해놓는다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_InitWeaponMesh()
{
	for( const auto& [num, weaponInfo] : GetDataInfoManager().GetWeaponInfos() )
	{
		auto staticMesh = Cast<UStaticMeshComponent>( OwningCharacter->GetDefaultSubobjectByName( weaponInfo.ComponentName ) );
		if( staticMesh )
			WeaponMeshes.Add( num, staticMesh );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 디졸브 애니메이션을 종료 처리한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_DissovleAnimEnd()
{
	if ( !CurWeaponMesh )
		return;

	auto curMesh = OwningCharacter->GetMesh();
	if( !curMesh )
		return;

	DissolveAmount = 0.f;

	// 무기 머티리얼을 현재 캐릭터 머티리얼과 동일한 머티리얼로 바꿔준다.
	CurWeaponMesh->SetMaterial( 0, curMesh->GetMaterial( 0 ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 무기 생성 쿨타임을 등록한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_RegisterCoolTime( const FWeaponInfo& InWeaponInfo )
{
	if( InWeaponInfo.CoolTime <= 0 )
		return;

	if( float* coolTime = CoolingWeapons.Find( InWeaponInfo.State ) )
		*coolTime = InWeaponInfo.CoolTime;
	else
		CoolingWeapons.Add( InWeaponInfo.State, InWeaponInfo.CoolTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 무기 생성 쿨타임을 돌린다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UWeaponComp::_CoolingWeapons( float InDeltaTime )
{
	for( auto& [_, remainCoolTime] : CoolingWeapons )
	{
		if( remainCoolTime > 0 )
			remainCoolTime -= InDeltaTime;
		else
			continue;

		if( remainCoolTime < 0 )
			remainCoolTime = 0;
	}
}