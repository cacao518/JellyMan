// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingBarComp.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UFloatingBarComp : public UWidgetComponent
{
	GENERATED_BODY()

private:
	class AActor* OwningActor;         

	class UUserWidget*  UserWidget;
	class UProgressBar* HpProgressBar;

	float ShowTime;

public:
	// ������ �Լ�
	UFloatingBarComp();

	// �Ҹ��� �Լ�
	virtual ~UFloatingBarComp();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// HP�� ������Ʈ �Լ�
	void UpdateHpBar( float InPercent );
};
