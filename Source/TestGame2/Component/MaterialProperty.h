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
	EMaterialState      MatState;       	     // 물질 상태

private:
	ACharacter*         OwningCharacter;         // 부모 캐릭터 클래스
	TArray<UMaterialInterface*> Materials;       // 모든 머터리얼 애셋

	bool            IsEnabledTileColl;            // 타일 콜리전 활성화 여부
	bool            FallingShakeToWeightOnce;     // 무게에 의한 흔들림을 한번만 시도하기위한 변수

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
	void SetMatState( EMaterialState InMatState, bool InChangeAnim = false );

	// 타일 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledTileColl( bool InIsEnabled );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// 현재 물질 상태를 반환한다.
	EMaterialState GetMatState() { return MatState; };

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////

	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void TileCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// 머터리얼 애셋 주소를 저장한다.
	void _InitMatAssets();

	// 머티리얼에 맞는 능력치를 초기화한다.
	void _InitStatus();

	// 머터리얼 애셋 주소를 EMaterialState로 바꿔준다.
	EMaterialState _ConvertMatAssetToMatState( UMaterialInterface* InMaterial );

	// 무거운 상태 로직을 실행한다.
	void _ProcessHeavyMaterial();
};
