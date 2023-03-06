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
	// 생성자 함수
	UFloatingBarComp();

	// 소멸자 함수
	virtual ~UFloatingBarComp();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// HP바 업데이트 함수
	void UpdateHpBar( float InPercent );
};
