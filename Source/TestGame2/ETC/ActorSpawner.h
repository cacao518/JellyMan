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
	UClass* Actor;                  ///< ������ ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	float  SpawnInterval;           ///< ���� ���� ( �ش� �� �������� �ݺ� ���� )

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	int    SpawnCountMaxTotal;      ///< ���� �ִ� �� ( ��ü )

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	int    SpawnCountMaxInWorld;    ///< ���� �ִ� �� ( �� �ʿ��� ) 

private:
	float  SpawnIntervalCount;     ///< ���� ���� ī��Ʈ
	int    SpawnCountTotal;        ///< ���� �� ( ��ü )
	int    SpawnCountInWorld;      ///< ���� �� ( �� �ʿ��� ) 
	
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
};
