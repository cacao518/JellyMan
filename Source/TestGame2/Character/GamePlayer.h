// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../ETC/SDB.h"
#include <functional>
#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayer.generated.h"


class UAnimMontage;
class USkeletalMeshComponent;


using InputMap = map<EInputKeyType, function<void()> >;


UCLASS(config=Game)
class AGamePlayer final : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UGameObject* GameObject;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UMaterialProperty* MatProperty;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UWeaponChange* WeaponChange;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* AttackColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* TileColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

private:
	InputMap              InputTypeAndFuncMap;  // Key:�Է�Ű����, Value:�Է�Ű �Լ�
	float                 ReadySkillResetTime;  // �ߵ� ������� ��ų �ʱ�ȭ �ð�
	function<void()>      ReadySkillFunc;       // �ߵ� ������� ���������� �Է��� ��ų �Լ�
	EInputKeyType         ReadySkillInputKey;   // �ߵ� ������� ���������� �Է��� Ű ����
	ACharacter*           LockOnTarget;         // ���� �� ���

public:
	AGamePlayer();

	virtual void BeginPlay() override;

	virtual void Tick(float InDeltaTime) override;

	virtual void Jump() override;

	void ProcessLeftMouse();
	void ProcessRightMouse();
	void ProcessBothMouseDown();
	void ProcessBothMouseUp();
	void ProcessWheel();
	void ProcessSpace();
	void ProcessF();
	void ProcessR();
	void Process1();

	// ���� �� ����� ��ȯ�Ѵ�.
	ACharacter* GetLockOnTarget() { return LockOnTarget; };

private:
	// �ߵ� ����� ��ų �ʱ�ȭ
	void _ResetReadySkill();

	// �ߵ� ����� ��ų ����
	void _SetReadySkill( EInputKeyType InReadyInputKey );

	// �Ͽ� ����
	void _LockOnStart();

	// �Ͽ� ����
	void _LockOnRelease();

	///////////////////////////////////////////////////////////////////////
	// Update
	///////////////////////////////////////////////////////////////////////

	// �ߵ� ������� ��ų ����
	void _ProcessReadySkill( float InDeltaTime );

	// ���� ��� ����
	void _ProcessLockOn();

	// ȸ�� ���� ���� �����Ѵ�.
	void _ProcessRotationRate();
};
