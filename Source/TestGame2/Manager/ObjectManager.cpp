
#include "ObjectManager.h"
#include "Engine/World.h"
#include "../System/MyGameInstance.h"
#include "GameFramework/Actor.h"

ObjectManager* ObjectManager::Instance = nullptr;

ObjectManager::ObjectManager()
{
	ObjectId = 0;
}

ObjectManager::~ObjectManager()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ƽ �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::Tick( float InDeltaTime )
{
	for( AActorSpawner* spawner : SpawnerList )
	{
		spawner->AddSpawnIntervalCount( InDeltaTime );
		if( spawner->GetSpawnInterval() <= spawner->GetSpawnIntervalCount() )
		{
			SpawnActor( spawner->GetActor(), spawner->GetActorLocation(), spawner->GetActorRotation() );
			spawner->AddSpawnCountTotal();
			spawner->AddSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
AActor* ObjectManager::SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator )
{
	if( !GameInstance )
		return nullptr;

	if( Objects.find( ObjectId ) != Objects.end() )
	{
		AActor* actor = Objects.find( ObjectId )->second;
		actor->SetActorLocationAndRotation( InLocation, InRotator );

		return actor;
	}
	else
	{
		//FString path = TEXT( "/Game/Monster/" );
		//UClass* pClass = LoadClass< AActor >( path );
		//if( !pClass )
		//	return nullptr;

		UWorld* World = GameInstance->GetWorld();
		if( !World )
			return nullptr;

		AActor* actor = World->SpawnActor< AActor >( InClass, InLocation, InRotator );
		if( !actor )
			return nullptr;

		Objects[ ObjectId++ ] = actor;

		return actor;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ��� �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::RegisterSpawner( AActorSpawner* InSpawner )
{
	if( !InSpawner )
		return;

	SpawnerList.push_back( InSpawner );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ����Ʈ�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::ClearSpawnerList()
{
	SpawnerList.clear();
}