// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UWeaponComp final : public UActorComponent
{
	GENERATED_BODY()

public:
	using WeaponMeshMap = TMap< int, UStaticMeshComponent* >;
	using CooltimeMap   = TMap<EWeaponState, float>; // (key:무기종류, value:남은쿨타임)
	using WeaponNumMap  = TMap<EWeaponState, int>;   // (Key:무기종류, value:무기식별자)

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	EWeaponState                    WeaponState;       	         // 무기 상태

private:
	ACharacter*                     OwningCharacter;             // 부모 캐릭터 클래스

	WeaponNumMap                    RegisteredWeaponNums;        // 등록된 무기 식별자
	WeaponMeshMap                   WeaponMeshes;                // 무기 스태틱 메시 애셋들
	CooltimeMap                     CoolingWeapons;              // 쿨타임 돌고 있는 무기 정보		

	int                             CurWeaponNum;                // 현재 무기 식별자
	class UStaticMeshComponent*     CurWeaponMesh;               // 현재 무기 스태틱매쉬
	class UMaterialInstanceDynamic* DissovleMaterialInstance;    // 디졸브 머티리얼 인스턴스
	UMaterialInterface*             DissolveMaterial;            // 디졸브 머티리얼 주소
	float                           DissolveAmount;              // 디졸브 변환 값

	int                             WeaponDurability;            // 무기 내구도
	int                             WeaponDurabilityMax;         // 무기 내구도 최대

public:	
	UWeaponComp();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// 무기 소환 가능한지 여부
	bool CanWeaponComp( EWeaponState InWeaponState );

	// 무기 내구도를 감소시킨다.
	void SubWeaponDurability();
	
	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// 무기를 변경한다.
	void EquipWeapon( EWeaponState InWeaponState, bool InChangeAnim = true );

	// 무기를 해제한다.
	void UnEquipWeapon();

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// 현재 무기 상태를 반환한다.
	EWeaponState GetWeaponState() { return WeaponState; };

	// 현재 무기 식별자를 반환한다.
	int GetCurWeaponNum() { return CurWeaponNum; };

	// 현재 무기 스태틱매쉬를 반환한다.
	UStaticMeshComponent* GetCurWeaponMesh() { return CurWeaponMesh; };

private:
	// 무기 메쉬 주소를 저장해놓는다.
	void _InitWeaponMesh();

	// 디졸브 애니메이션을 종료 처리한다.
	void _DissovleAnimEnd();

	// 무기 생성 쿨타임을 등록한다.
	void _RegisterCoolTime( const FWeaponInfo& InWeaponInfo );

	// 무기 생성 쿨타임을 돌린다.
	void _CoolingWeapons( float InDeltaTime );
};
