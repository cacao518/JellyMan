// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


class UUIBase;


class UIManager final : public SingletonBase< UIManager>
{

public:
	UIManager();
	~UIManager();

	// ƽ �Լ�
	void Tick( float InDeltaTime );

	// UI�� �����ϰ� ����.
	UUIBase* ShowUI( const FString& InPath );

};
inline UIManager& GetUIManager() { return UIManager::GetInstance(); };