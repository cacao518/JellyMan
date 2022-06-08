// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/ActorSpawner.h"
#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

using namespace std;

class AActorSpawner;
class UMyGameInstance;

typedef unordered_map<int, AActor*> ActorMap;
typedef list< AActorSpawner* > ActorSpawnerList;

class ObjectManager
{
private:
	UPROPERTY()
	ActorMap         Objects;

	UPROPERTY()
	ActorSpawnerList SpawnerList;

	UPROPERTY()
	UMyGameInstance* GameInstance;
	
	int ObjectId;

public:
	ObjectManager();
	~ObjectManager();

	// ƽ �Լ�
	void Tick( float InDeltaTime );

	///////////////////////////////////////////////////////////////
	/// ������Ʈ
	///////////////////////////////////////////////////////////////
	
	// ���� ����
	AActor* SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator );


	///////////////////////////////////////////////////////////////
	/// ������
	///////////////////////////////////////////////////////////////
	
	// ������ ��� �Լ�
	void RegisterSpawner( AActorSpawner* InSpawner);

	// ������ ����Ʈ�� �����Ѵ�.
	void ClearSpawnerList();

	///////////////////////////////////////////////////////////////
	/// �̱��� �ڵ�
	///////////////////////////////////////////////////////////////
	static class ObjectManager* Instance;
	static ObjectManager& GetInstance(){ return *Instance; }
	static ObjectManager* CreateInstance() { return !Instance ? Instance = new ObjectManager() : Instance; }
	static void DestroyInstance() { if( !Instance ) delete Instance; Instance = nullptr; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief  LoadClass
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template< typename T >
	UClass* LoadClass( const FString& InPath )
	{
		FString copiedPath = InPath;
		return ConstructorHelpersInternal::FindOrLoadClass( copiedPath, T::StaticClass() );
	}
};
inline ObjectManager& GetObjectManager() { return ObjectManager::GetInstance(); };