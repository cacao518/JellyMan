// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaterialProperty.generated.h"


class UMaterialInterface;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UMaterialProperty : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	EMaterialState      MatState;       	     // ���� ����

private:
	ACharacter*         OwningCharacter;         // �θ� ĳ���� Ŭ����
	TArray<UMaterialInterface*> Materials;       // ��� ���͸��� �ּ�

	bool            IsEnabledTileColl;            // Ÿ�� �ݸ��� Ȱ��ȭ ����
	bool            FallingShakeToWeightOnce;     // ���Կ� ���� ��鸲�� �ѹ��� �õ��ϱ����� ����

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
	void SetMatState( EMaterialState InMatState, bool InChangeAnim = false );

	// Ÿ�� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledTileColl( bool InIsEnabled );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EMaterialState GetMatState() { return MatState; };

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////

	// �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
	UFUNCTION()
	void TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// ���͸��� �ּ� �ּҸ� �����Ѵ�.
	void _InitMatAssets();

	// ��Ƽ���� �´� �ɷ�ġ�� �ʱ�ȭ�Ѵ�.
	void _InitStatus();

	// ���͸��� �ּ� �ּҸ� EMaterialState�� �ٲ��ش�.
	EMaterialState _ConvertMatAssetToMatState( UMaterialInterface* InMaterial );

	// ���ſ� ���� ������ �����Ѵ�.
	void _ProcessHeavyMaterial();
};
