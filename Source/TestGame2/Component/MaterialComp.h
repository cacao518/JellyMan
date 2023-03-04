// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaterialComp.generated.h"


class UMaterialInterface;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UMaterialComp final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UMaterialInterface* InitMaterial;             // �ʱ� ����

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	EMaterialState      MatState;       	      // ���� ����

private:
	ACharacter*         OwningCharacter;          // �θ� ĳ���� Ŭ����

	float               JellyEnergy;              // ���� ������
	float               JellyEnergyMax;           // ���� ������ �ִ�
	float               MatEnergy;                // ���� ������
	float               MatEnergyMax;             // ���� ������ �ִ�

	bool                IsEnabledTileColl;        // Ÿ�� �ݸ��� Ȱ��ȭ ����

public:	
	UMaterialComp();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// ������ �����Ѵ�.
	void SetMatState( UMaterialInterface* InMatInterface = nullptr, bool InIsInit = false );

	// ���� �������� �����Ѵ�.
	void SetJellyEnergy( float InJellyEnergy ){ JellyEnergyMax > InJellyEnergy ? JellyEnergy = InJellyEnergy : JellyEnergy = JellyEnergyMax; };

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EMaterialState GetMatState() { return MatState; };

	// ���� ���� �������� ��ȯ�Ѵ�.
	float GetJellyEnergy() { return JellyEnergy; };

	// ���� ���� ������ �ƽ��� ��ȯ�Ѵ�.
	float GetJellyEnergyMax() { return JellyEnergyMax; };

	// �߰� ���� ���� ���θ� ��ȯ�Ѵ�.
	bool IsMiddleIntensity();

	// �� ���� ���� ���θ� ��ȯ�Ѵ�.
	bool IsHardIntensity();

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// ��Ƽ���� �´� �ɷ�ġ�� �ʱ�ȭ�Ѵ�.
	void _InitStatus();

	// ���� ���� ����Ʈ�� �����Ѵ�.
	void _PlayChangeEffect();

	// ������ ���� ������ �����Ѵ�.
	void _ProcessGauge( float InDeltaTime );

	// Ǯ���� ���� ������ �����Ѵ�.
	void _ProcessGrass( float InDeltaTime );
};
