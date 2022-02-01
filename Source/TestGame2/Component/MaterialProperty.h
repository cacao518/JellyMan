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
	ACharacter* OwningCharacter;         // �θ� ĳ���� Ŭ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EMaterialState      MatState;       	     // ���� ����

public:	
	UMaterialProperty();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ���� �ö� �ִ� Ÿ���� ������ �����Ѵ�.
	void CopyMaterial();
	
	// ������ �����Ѵ�.
	void SetMatState( EMaterialState InMatState, bool InChangeAnim = false );

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EMaterialState GetMatState(){ return MatState; };
};
