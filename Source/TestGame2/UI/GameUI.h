// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBase.h"
#include "GameUI.generated.h"


class UProgressBar;
class UCharacterComp;
class UMaterialComp;


UCLASS()
class TESTGAME2_API UGameUI : public UUIBase
{
	GENERATED_BODY()
	
private:
	UProgressBar* ProgressBarHP;         // HP 프로그레스바
	UProgressBar* ProgressBarJelly;      // Jelly 프로그레스바

	UMaterialComp* MyPlayerMatComp;      // 내캐릭터 MaterialComp
	UCharacterComp* MyPlayerCharComp;     // 내캐릭터 CharacterComp

public:
	// NativeConstruct
	virtual void NativeConstruct() override;

	// NativeTick
	virtual void NativeTick( const FGeometry& MyGeometry, float InDeltaTime ) override;

	// BeginDestroy
	virtual void BeginDestroy() override;

protected:
	// UI 생성 완료됨을 알린다.
	virtual void OnCreated() override;

private:
	// Hp바를 업데이트한다.
	void _UpdateHpBar();

	// 젤리바를 업데이트한다.
	void _UpdateJellyBar();
};
