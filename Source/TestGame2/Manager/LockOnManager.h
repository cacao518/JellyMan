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
	ACharacter* LockOnTarget;         // 락온 된 대상 

public:
	LockOnManager();
	~LockOnManager();

	// 틱 함수
	void Tick( float InDeltaTime );

	// 록온 시작
	void LockOnStart( ELockOnMode InMode = ELockOnMode::Default );

	// 록온 해제
	void LockOnRelease();

	// 락온 된 대상을 반환한다.
	ACharacter* GetLockOnTarget() { return LockOnTarget; };

private:
	// 락온 기능 수행
	void _ProcessLockOn();

};
inline LockOnManager& GetLockOnManager() { return LockOnManager::GetInstance(); };