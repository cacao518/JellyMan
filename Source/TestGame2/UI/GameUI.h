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
	UProgressBar* ProgressBarHP;         // HP ���α׷�����
	UProgressBar* ProgressBarMP;         // MP ���α׷�����

	UProgressBar* ProgressBarWeapon1;    // ����1 ��Ÿ�� ���α׷�����
	UProgressBar* ProgressBarWeapon2;    // ����2 ��Ÿ�� ���α׷�����
	UProgressBar* ProgressBarWeapon3;    // ����3 ��Ÿ�� ���α׷�����

	UCharacterComp* MyPlayerCharComp;    // ��ĳ���� CharacterComp
	UWeaponComp* MyPlayerWeaponComp;     // ��ĳ���� CharacterComp

public:
	// NativeConstruct
	virtual void NativeConstruct() override;

	// NativeTick
	virtual void NativeTick( const FGeometry& MyGeometry, float InDeltaTime ) override;

	// BeginDestroy
	virtual void BeginDestroy() override;

protected:
	// UI ���� �Ϸ���� �˸���.
	virtual void OnCreated() override;

private:
	// ���α׷����ٸ� ������Ʈ�Ѵ�.
	void _UpdateProgressBar();
};
