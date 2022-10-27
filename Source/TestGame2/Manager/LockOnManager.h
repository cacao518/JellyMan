// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


class LockOnManager final : public SingletonBase< LockOnManager>
{

public:
	LockOnManager();
	~LockOnManager();

	// ƽ �Լ�
	void Tick( float InDeltaTime );

};
inline LockOnManager& GetLockOnManager() { return LockOnManager::GetInstance(); };