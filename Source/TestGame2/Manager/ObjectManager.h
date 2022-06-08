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

	// 틱 함수
	void Tick( float InDeltaTime );

	///////////////////////////////////////////////////////////////
	/// 오브젝트
	///////////////////////////////////////////////////////////////
	
	// 액터 생성
	AActor* SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator );


	///////////////////////////////////////////////////////////////
	/// 스포너
	///////////////////////////////////////////////////////////////
	
	// 스포너 등록 함수
	void RegisterSpawner( AActorSpawner* InSpawner);

	// 스포너 리스트를 정리한다.
	void ClearSpawnerList();

	///////////////////////////////////////////////////////////////
	/// 싱글톤 코드
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