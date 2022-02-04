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
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	ACharacter* OwningCharacter;         // �θ� ĳ���� Ŭ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EMaterialState      MatState;       	     // ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EMaterialState      MatStateOnTile;       	 // ��� �ִ� Ÿ�� �� ���� ����

	TArray<UMaterialInterface*> Materials;       // ��� ���͸��� �ּ�

public:	
	UMaterialProperty();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ���� �ö� �ִ� Ÿ���� ������ �����Ѵ�.
	void CopyMaterial();

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// ������ �����Ѵ�.
	void SetMatState( EMaterialState InMatState, bool InChangeAnim = false );

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

	// ���͸��� �ּ� �ּҸ� EMaterialState�� �ٲ��ش�.
	EMaterialState _ConvertMatAssetToMatState( UMaterialInterface* InMaterial );
};