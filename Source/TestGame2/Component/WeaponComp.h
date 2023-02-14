// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"


using WeaponMeshMap = TMap< EWeaponState, UStaticMeshComponent* >;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UWeaponComp final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	EWeaponState                    WeaponState;       	         // ���� ����

private:
	ACharacter*                     OwningCharacter;             // �θ� ĳ���� Ŭ����
	WeaponMeshMap                   WeaponMeshes;                // ���� ����ƽ �޽� �ּµ�
	class UStaticMeshComponent*     CurWeaponMesh;               // ���� ���� ����ƽ�Ž�

	class UMaterialInstanceDynamic* DissovleMaterialInstance;    // ������ ��Ƽ���� �ν��Ͻ�
	UMaterialInterface*             DissolveMaterial;            // ������ ��Ƽ���� �ּ�
	float                           DissolveAmount;              // ������ ��ȯ ��

	int                             WeaponDurability;            // ���� ������
	int                             WeaponDurabilityMax;         // ���� ������ �ִ�

public:	
	UWeaponComp();

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

	// ���� ���� ����ƽ�Ž��� ��ȯ�Ѵ�.
	UStaticMeshComponent* GetCurWeaponMesh() { return CurWeaponMesh; };

private:
	// ���� �޽� �ּҸ� �����س��´�.
	void _InitWeaponMesh();

	// ������ �ִϸ��̼��� ���� ó���Ѵ�.
	void _DissovleAnimEnd();

};
