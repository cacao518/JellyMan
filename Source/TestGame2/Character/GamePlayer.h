// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../ETC/SDB.h"
#include <functional>
#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayer.generated.h"

using namespace std;

class UAnimMontage;
class USkeletalMeshComponent;


typedef map<EInputKeyType, function<void()> > InputMap;


UCLASS(config=Game)
class AGamePlayer : public ACharacter
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

public:
	AGamePlayer();

	virtual void BeginPlay() override;

	virtual void Tick(float InDeltaTime) override;

	virtual void Jump() override;

	void ProcessLeftMouse();
	void ProcessRightMouse();
	void ProcessSpace();
	void ProcessF();
	void ProcessR();
	void Process1();

	bool Punch1Start();
	bool Punch2Start();
	bool Punch3Start();
	bool SwordAttack1Start();
	bool SwordAttack2Start();
	bool SwordAttack3Start();

private:
	// ���� ��ȯ �������� ����
	bool _CanWeaponChange( EWeaponState InWeaponState );

	// �ߵ� ����� ��ų �ʱ�ȭ
	void _ResetReadySkill();

	// �ߵ� ����� ��ų ����
	void _SetReadySkill( EInputKeyType InReadyInputKey );

	// �ߵ� ������� ��ų ����
	void _ProcessReadySkill( float InDeltaTime );
};
