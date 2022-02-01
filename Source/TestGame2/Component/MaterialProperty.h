// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaterialProperty.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UMaterialProperty : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	ACharacter* OwningCharacter;         // 부모 캐릭터 클래스

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EMaterialState      MatState;       	     // 물질 상태

public:	
	UMaterialProperty();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// 현재 올라가 있는 타일의 물질을 복사한다.
	void CopyMaterial();
	
	// 물질을 변경한다.
	void SetMatState( EMaterialState InMatState, bool InChangeAnim = false );

	// 현재 물질 상태를 반환한다.
	EMaterialState GetMatState(){ return MatState; };
};
