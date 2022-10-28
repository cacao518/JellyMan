// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


class LockOnManager final : public SingletonBase< LockOnManager>
{

private:
	ACharacter* LockOnTarget;         // ���� �� ���

public:
	LockOnManager();
	~LockOnManager();

	// ƽ �Լ�
	void Tick( float InDeltaTime );

	// �Ͽ� ����
	void LockOnStart();

	// �Ͽ� ����
	void LockOnRelease();

	// ���� �� ����� ��ȯ�Ѵ�.
	ACharacter* GetLockOnTarget() { return LockOnTarget; };

private:
	// ���� ��� ����
	void _ProcessLockOn();

};
inline LockOnManager& GetLockOnManager() { return LockOnManager::GetInstance(); };