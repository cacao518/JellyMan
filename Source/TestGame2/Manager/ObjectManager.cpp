
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
//// @brief 틱 함수
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
//// @brief 액터 생성
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
//// @brief 스포너 등록 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::RegisterSpawner( AActorSpawner* InSpawner )
{
	if( !InSpawner )
		return;

	SpawnerList.push_back( InSpawner );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스포너 리스트를 정리한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::ClearSpawnerList()
{
	SpawnerList.clear();
}