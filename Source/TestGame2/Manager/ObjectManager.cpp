
#include "ObjectManager.h"
#include "Engine/World.h"
#include "../System/MyGameInstance.h"
#include "../Component/GameObject.h"
#include "GameFramework/Actor.h"

ObjectManager* ObjectManager::Instance = nullptr;

ObjectManager::ObjectManager()
{
	ObjectId = 0;
}

ObjectManager::~ObjectManager()
{
	ClearSpawnerList();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 틱 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::Tick( float InDeltaTime )
{
	SpawnActorInSpawner( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 액터 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
AActor* ObjectManager::SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator, AActorSpawner* InSpawner )
{
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

		UWorld* World = GetMyGameInstance().GetWorld();
		if( !World )
			return nullptr;

		AActor* actor = World->SpawnActor< AActor >( InClass, InLocation, InRotator );
		if( !actor )
			return nullptr;

		auto gameObject = actor ? Cast<UGameObject>( actor->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
		if( gameObject )
			gameObject->SetId( ObjectId );

		Objects[ ObjectId ] = actor;
		SpawnerMap[ ObjectId ] = InSpawner;

		ObjectId++;
		return actor;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 액터 제거 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::DestroyActor( AActor* InActor )
{
	auto gameObject = InActor ? Cast<UGameObject>( InActor->GetDefaultSubobjectByName( TEXT( "GameObject" ) ) ) : nullptr;
	if( !gameObject )
		return;

	if( Objects.find( gameObject->GetId() ) != Objects.end() )
	{
		InActor->Destroy();
		Objects.erase( gameObject->GetId() );

		if( AActorSpawner* spawner = SpawnerMap[ gameObject->GetId() ] )
		{
			spawner->SubSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
	else
	{
		if( InActor )
			InActor->Destroy();
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
//// @brief 스포너에서 스폰한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::SpawnActorInSpawner( float InDeltaTime )
{
	for( AActorSpawner* spawner : SpawnerList )
	{
		spawner->AddSpawnIntervalCount( InDeltaTime );
		if( spawner->GetSpawnInterval() <= spawner->GetSpawnIntervalCount() )
		{
			if( spawner->GetSpawnCountTotal() >= spawner->GetSpawnCountMaxTotal() ||
				spawner->GetSpawnCountInWorld() >= spawner->GetSpawnCountMaxInWorld() )
				continue;

			SpawnActor( spawner->GetActor(), spawner->GetActorLocation(), spawner->GetActorRotation(), spawner );
			spawner->AddSpawnCountTotal();
			spawner->AddSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스포너 리스트를 정리한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::ClearSpawnerList()
{
	SpawnerList.clear();
	SpawnerMap.clear();
}