// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Manager/ObjectManager.h"
#include "GameFramework/Actor.h"
#include "ActorSpawner.generated.h"

UCLASS()
class TESTGAME2_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UClass* Actor;                  ///< 스폰할 액터

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	float  SpawnInterval;           ///< 스폰 간격 ( 해당 초 간격으로 반복 생성 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	int    SpawnCountMaxTotal;      ///< 스폰 최대 수 ( 전체 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	int    SpawnCountMaxInWorld;    ///< 스폰 최대 수 ( 한 맵에서 )

private:
	float  SpawnIntervalCount;     ///< 스폰 간격 카운트
	int    SpawnCountTotal;        ///< 스폰 수 ( 전체 )
	int    SpawnCountInWorld;      ///< 스폰 수 ( 한 맵에서 ) 
	
public:	
	AActorSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UClass* GetActor(){ return Actor; };
	int GetSpawnInterval(){ return SpawnInterval; };
	int GetSpawnCountMaxTotal(){ return SpawnCountMaxTotal; };
	int GetSpawnCountMaxInWorld(){ return SpawnCountMaxInWorld; };

	float GetSpawnIntervalCount(){ return SpawnIntervalCount; };
	int GetSpawnCountTotal(){ return SpawnCountTotal; };
	int GetSpawnCountInWorld(){ return SpawnCountInWorld; };

	void AddSpawnIntervalCount( float InDeltaTime ){ SpawnIntervalCount += InDeltaTime; };
	void ResetSpawnIntervalCount(){ SpawnIntervalCount = 0; };
	void AddSpawnCountTotal(){ SpawnCountTotal++; };
	void AddSpawnCountInWorld(){ SpawnCountInWorld++; };
	void SubSpawnCountInWorld(){ SpawnCountInWorld--; };
};
