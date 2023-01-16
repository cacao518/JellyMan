// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

enum class ELockOnMode : uint8
{
	Default,
	Prev,
	Next,
	Max
};

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
	void LockOnStart( ELockOnMode InMode = ELockOnMode::Default );

	// �Ͽ� ����
	void LockOnRelease();

	// ���� �� ����� ��ȯ�Ѵ�.
	ACharacter* GetLockOnTarget() { return LockOnTarget; };

private:
	// ���� ��� ����
	void _ProcessLockOn();

};
inline LockOnManager& GetLockOnManager() { return LockOnManager::GetInstance(); };