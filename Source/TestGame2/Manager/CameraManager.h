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

	// ƽ �Լ�
	void Tick( float InDeltaTime );

	// ī�޶� ����ũ�� �����Ѵ�.
	void CameraShake( float InScale = 1.f, bool InShakeByWeight = false );

private:
	// ī�޶� �� ������ �����Ѵ�.
	void _ProcessCameraArm( float InDeltaTime );

};
inline CameraManager& GetCameraManager() { return CameraManager::GetInstance(); };