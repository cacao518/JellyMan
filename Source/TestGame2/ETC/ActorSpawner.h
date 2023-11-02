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
	UClass* ActorClass;              ///< ������ ����

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

	// ���� �������� ���θ� Ȯ���Ѵ�.
	bool CanSpawn();

	// ��ȯ�� ���͸� ��ȯ�Ѵ�.
	UClass* GetActorClass(){ return ActorClass; };

	void AddSpawnIntervalCount( float InDeltaTime ){ SpawnIntervalCount += InDeltaTime; };
	void ResetSpawnIntervalCount(){ SpawnIntervalCount = 0; };
	void AddSpawnCountTotal(){ SpawnCountTotal++; };
	void AddSpawnCountInWorld(){ SpawnCountInWorld++; };
	void SubSpawnCountInWorld(){ SpawnCountInWorld--; };
};
