
#include "ActorSpawner.h"
#include "../Manager/ObjectManager.h"

AActorSpawner::AActorSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnCountTotal = 0;
	SpawnCountInWorld = 0;
}

void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetObjectManager().RegisterSpawner( this );
}

void AActorSpawner::BeginDestroy()
{
	Super::BeginDestroy();
}

void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스폰 가능한지 여부를 확인한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool AActorSpawner::CanSpawn()
{
	if( SpawnInterval <= SpawnIntervalCount )
	{
		if( SpawnCountTotal < SpawnCountMaxTotal && SpawnCountInWorld < SpawnCountMaxInWorld )
			return true;
	}

	return false;
}
