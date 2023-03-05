// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBase.h"
#include "GameUI.generated.h"


class UProgressBar;
class UCharacterComp;
class UWeaponComp;


UCLASS()
class TESTGAME2_API UGameUI : public UUIBase
{
	GENERATED_BODY()
	
private:
	UProgressBar* ProgressBarHP;         // HP 프로그레스바
	UProgressBar* ProgressBarMP;         // MP 프로그레스바

	UProgressBar* ProgressBarWeapon1;    // 무기1 쿨타임 프로그레스바
	UProgressBar* ProgressBarWeapon2;    // 무기2 쿨타임 프로그레스바
	UProgressBar* ProgressBarWeapon3;    // 무기3 쿨타임 프로그레스바

	UCharacterComp* MyPlayerCharComp;    // 내캐릭터 CharacterComp
	UWeaponComp* MyPlayerWeaponComp;     // 내캐릭터 CharacterComp

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
	// 프로그레스바를 업데이트한다.
	void _UpdateProgressBar();
};
