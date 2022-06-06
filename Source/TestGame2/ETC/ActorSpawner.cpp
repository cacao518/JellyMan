
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

