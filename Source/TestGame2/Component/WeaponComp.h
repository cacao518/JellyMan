// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"


using WeaponMeshMap     = TMap< int, UStaticMeshComponent* >;
using WeaponCooltimeMap = TMap<EWeaponState, float>; // (key:��������, value:������Ÿ��)
using WeaponNumMap      = TMap<EWeaponState, int>;   // (Key:��������, value:����ĺ���)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UWeaponComp final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	EWeaponState                    WeaponState;       	         // ���� ����

private:
	ACharacter*                     OwningCharacter;             // �θ� ĳ���� Ŭ����

	WeaponNumMap                    RegisteredWeaponNums;        // ��ϵ� ���� �ĺ���
	WeaponMeshMap                   WeaponMeshes;                // ���� ����ƽ �޽� �ּµ�
	WeaponCooltimeMap               CoolingWeapons;              // ��Ÿ�� ���� �ִ� ���� ����		

	int                             CurWeaponNum;                // ���� ���� �ĺ���
	class UStaticMeshComponent*     CurWeaponMesh;               // ���� ���� ����ƽ�Ž�
	class UMaterialInstanceDynamic* DissovleMaterialInstance;    // ������ ��Ƽ���� �ν��Ͻ�
	UMaterialInterface*             DissolveMaterial;            // ������ ��Ƽ���� �ּ�
	float                           DissolveAmount;              // ������ ��ȯ ��

	int                             WeaponDurability;            // ���� ������
	int                             WeaponDurabilityMax;         // ���� ������ �ִ�

public:	
	// ������
	UWeaponComp();

	// �Ҹ���
	virtual ~UWeaponComp();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ���� ��ȯ �������� ����
	bool CanWeaponComp( EWeaponState InWeaponState );

	// ���� �������� ���ҽ�Ų��.
	void SubWeaponDurability();
	
	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// ���⸦ �����Ѵ�.
	void EquipWeapon( EWeaponState InWeaponState, bool InChangeAnim = true );

	// ���⸦ �����Ѵ�.
	void UnEquipWeapon();

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EWeaponState GetWeaponState() { return WeaponState; };

	// ���� ���� �ĺ��ڸ� ��ȯ�Ѵ�.
	int GetCurWeaponNum() { return CurWeaponNum; };

	// ���� ���� ����ƽ�Ž��� ��ȯ�Ѵ�.
	UStaticMeshComponent* GetCurWeaponMesh() { return CurWeaponMesh; };

	// �ش� ���� ��Ÿ�� �ۼ�Ʈ�� ��ȯ�Ѵ�.
	float GetWeaponCoolTimePercent( EWeaponState InWeaponState );

private:
	// ���� �޽� �ּҸ� �����س��´�.
	void _InitWeaponMesh();

	// ������ �ִϸ��̼��� ���� ó���Ѵ�.
	void _DissovleAnimEnd();

	// ���� ���� ��Ÿ���� ����Ѵ�.
	void _RegisterCoolTime( const FWeaponInfo& InWeaponInfo );

	// ���� ���� ��Ÿ���� ������.
	void _CoolingWeapons( float InDeltaTime );
};
