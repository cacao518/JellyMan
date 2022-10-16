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
	UMaterialInterface* InitMaterial;             // 초기 물질

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	EMaterialState      MatState;       	      // 물질 상태

private:
	ACharacter*         OwningCharacter;          // 부모 캐릭터 클래스
	UMaterialInterface* WaterMaterial;            // 워터 머터리얼

	float               JellyEnergy;              // 젤리 에너지
	float               JellyEnergyMax;           // 젤리 에너지 최대
	float               MatEnergy;                // 물질 에너지
	float               MatEnergyMax;             // 물질 에너지 최대

	bool                IsEnabledTileColl;        // 타일 콜리전 활성화 여부

public:	
	UMaterialProperty();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// 물질을 변경한다.
	void SetMatState( UMaterialInterface* InMatInterface = nullptr );

	// 타일 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledTileColl( bool InIsEnabled );

	// 젤리 에너지를 셋팅한다.
	void SetJellyEnergy( float InJellyEnergy ){ JellyEnergyMax > InJellyEnergy ? JellyEnergy = InJellyEnergy : JellyEnergy = JellyEnergyMax; };

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// 현재 물질 상태를 반환한다.
	EMaterialState GetMatState() { return MatState; };

	// 현재 젤리 에너지를 반환한다.
	float GetJellyEnergy() { return JellyEnergy; };

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////

	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// 초기화 한다.
	void _Init();

	// 머티리얼에 맞는 능력치를 초기화한다.
	void _InitStatus();

	// 머터리얼 애셋 주소를 EMaterialState로 바꿔준다.
	EMaterialState _ConvertMatAssetToMatState( UMaterialInterface* InMaterial );

	// 물질 변경 이펙트를 실행한다.
	void _PlayChangeEffect();

	// 게이지 관련 로직을 실행한다.
	void _ProcessGauge( float InDeltaTime );

	// 풀재질 관련 로직을 실행한다.
	void _ProcessGrass( float InDeltaTime );
};
