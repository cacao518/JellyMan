// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/ActorSpawner.h"
#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


class AActorSpawner;
class UNiagaraSystem;


using ActorMap         = unordered_map< int, AActor* >;
using SpawnerMap       = unordered_map< int, AActorSpawner* >;
using ActorSpawnerList = list< AActorSpawner* >;


class ObjectManager : public SingletonBase< ObjectManager>
{
private:
	UPROPERTY()
	ActorMap         Objects;      // 모든 오브젝트 리스트

	UPROPERTY()
	ActorSpawnerList SpawnerList;  // 모든 스포너 리스트

	UPROPERTY()
	SpawnerMap       SpawnerMap;   // key:소환된액터 아이디, value:스포너
	
	int ObjectId;                  // 오브젝트 고유 아이디

public:
	ObjectManager();
	~ObjectManager();

	// 틱 함수
	void Tick( float InDeltaTime );

	///////////////////////////////////////////////////////////////
	/// 오브젝트
	///////////////////////////////////////////////////////////////
	
	// 액터 생성
	AActor* SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator, AActorSpawner* InSpawner = nullptr );
	
	// 파티클 생성
	void SpawnParticle( UNiagaraSystem* InNiagara, const FVector& InLocation, const FRotator& InRotator );

	// 액터 제거
	void DestroyActor( AActor* InActor );

	// 리스트를 정리한다.
	void Clear();

	///////////////////////////////////////////////////////////////
	/// 스포너
	///////////////////////////////////////////////////////////////
	
	// 스포너 등록 함수
	void RegisterSpawner( AActorSpawner* InSpawner);

	// 스포너에서 스폰한다.
	void SpawnActorInSpawner( float InDeltaTime );

};
inline ObjectManager& GetObjectManager() { return ObjectManager::GetInstance(); };