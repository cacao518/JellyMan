// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


class CameraManager final : public SingletonBase< CameraManager>
{

public:
	CameraManager();
	~CameraManager();

	// 틱 함수
	void Tick( float InDeltaTime );

	// 카메라 쉐이크를 실행한다.
	void CameraShake( AActor* InCaster, float InScale = 1.f, bool InShakeByWeight = false, bool InShakeByIntensity = false );

private:
	// 카메라 암 로직을 수행한다.
	void _ProcessCameraArm( float InDeltaTime );

};
inline CameraManager& GetCameraManager() { return CameraManager::GetInstance(); };