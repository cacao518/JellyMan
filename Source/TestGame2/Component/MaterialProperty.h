// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaterialProperty.generated.h"


class UMaterialInterface;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UMaterialProperty final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UMaterialInterface* InitMaterial;             // �ʱ� ����

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	EMaterialState      MatState;       	      // ���� ����

private:
	ACharacter*         OwningCharacter;          // �θ� ĳ���� Ŭ����
	UMaterialInterface* WaterMaterial;            // ���� ���͸���

	float               JellyEnergy;              // ���� ������
	float               JellyEnergyMax;           // ���� ������ �ִ�
	float               MatEnergy;                // ���� ������
	float               MatEnergyMax;             // ���� ������ �ִ�

	bool                IsEnabledTileColl;        // Ÿ�� �ݸ��� Ȱ��ȭ ����

public:	
	UMaterialProperty();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// ������ �����Ѵ�.
	void SetMatState( UMaterialInterface* InMatInterface = nullptr );

	// Ÿ�� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledTileColl( bool InIsEnabled );

	// ���� �������� �����Ѵ�.
	void SetJellyEnergy( float InJellyEnergy ){ JellyEnergyMax > InJellyEnergy ? JellyEnergy = InJellyEnergy : JellyEnergy = JellyEnergyMax; };

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EMaterialState GetMatState() { return MatState; };

	// ���� ���� �������� ��ȯ�Ѵ�.
	float GetJellyEnergy() { return JellyEnergy; };

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////

	// �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
	UFUNCTION()
	void TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// ��Ƽ���� �´� �ɷ�ġ�� �ʱ�ȭ�Ѵ�.
	void _InitStatus();

	// ���͸��� �ּ� �ּҸ� EMaterialState�� �ٲ��ش�.
	EMaterialState _ConvertMatAssetToMatState( UMaterialInterface* InMaterial );

	// ���� ���� ����Ʈ�� �����Ѵ�.
	void _PlayChangeEffect();

	// ������ ���� ������ �����Ѵ�.
	void _ProcessGauge( float InDeltaTime );

	// Ǯ���� ���� ������ �����Ѵ�.
	void _ProcessGrass( float InDeltaTime );
};
