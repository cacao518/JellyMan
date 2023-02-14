// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <functional>
#include <map>
#include "MyPlayerController.generated.h"


class ACharacterPC;
class UCharacterComp;
class UMaterialComp;
class UWeaponComp;


using InputMap = map< EInputKeyType, function<void()> >;


UCLASS()
class TESTGAME2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	ACharacterPC*            MyPlayer;
	UCharacterComp*          CharacterComp;
	UMaterialComp*           MatComp;
	UWeaponComp*             WeaponComp;

	InputMap                 InputTypeAndFuncMap;  // Key:�Է�Ű����, Value:�Է�Ű �Լ�
	float                    ReadySkillResetTime;  // �ߵ� ������� ��ų �ʱ�ȭ �ð�
	function<void()>         ReadySkillFunc;       // �ߵ� ������� ���������� �Է��� ��ų �Լ�
	EInputKeyType            ReadySkillInputKey;   // �ߵ� ������� ���������� �Է��� Ű ����
						     
	float				     BaseTurnRate;
	float				     BaseLookUpRate;
	bool				     IsDownBothMoustButton;

public:
	AMyPlayerController();

public:
	virtual void BeginPlay() override;
	virtual void Tick( float InDeltaTime ) override;

	void MoveForward( float Value );
	void MoveRight( float Value );
	void TurnAtRate( float Rate );
	void LookUpAtRate( float Rate );
	void JumpStart();
	void JumpStop();

	void ProcessLeftMouse();
	void ProcessRightMouse();
	void ProcessBothMouseDown();
	void ProcessBothMouseUp();
	void ProcessWheelClick();
	void ProcessWheelUp();
	void ProcessWheelDown();
	void ProcessSpace();
	void ProcessF();
	void ProcessR();
	void Process1();

private:
	// ��ų�� ����Ѵ�.
	bool _SkillPlay( int InBasicSkillNum, int InMiddleSkillNum = 0, int InHardSkillNum = 0 );

	// �ߵ� ����� ��ų �ʱ�ȭ
	void _ResetReadySkill();

	// �ߵ� ����� ��ų ����
	void _SetReadySkill( EInputKeyType InReadyInputKey );

	// �ߵ� ������� ��ų ����
	void _ProcessReadySkill( float InDeltaTime );

	// ȸ�� ���� ���� �����Ѵ�.
	void _ProcessRotationRate();

	// ���� Ű �Է��� �޴´�.
	void _ProcessInputBothKey();
};
