
#include "ObjectManager.h"
#include "Engine/World.h"
#include "../Character/CharacterPC.h"
#include "../Character/CharacterNPC.h"
#include "../System/MyGameInstance.h"
#include "../Component/GameObject.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"


ObjectManager::ObjectManager()
{
	ObjectId = 0;
}

ObjectManager::~ObjectManager()
{
	Clear();
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
	if( AActor* actor = Objects.FindRef( ObjectId ) )
	{
		actor->SetActorLocationAndRotation( InLocation, InRotator );
		return actor;
	}
	else
	{
		//FString path = TEXT( "/Game/CharacterNPC/" );
		//UClass* pClass = LoadClass< AActor >( path );
		//if( !pClass )
		//	return nullptr;

		UWorld* World = GetMyGameInstance().GetWorld();
		if( !World )
			return nullptr;

		AActor* newActor = World->SpawnActor< AActor >( InClass, InLocation, InRotator );
		if( !newActor )
			return nullptr;

		auto gameObject = newActor ? Cast<UGameObject>( newActor->FindComponentByClass<UGameObject>() ) : nullptr;
		if( gameObject )
		{
			gameObject->SetId  ( ObjectId );
		}

		Objects.Add( ObjectId, newActor );
		SpawnerMap.Add( ObjectId, InSpawner );

		ObjectId++;
		return newActor;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 그라운드 오브젝트 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
AActor* ObjectManager::SpawnGroundObject( const FString& InName, const FVector& InLocation, const FRotator& InRotator )
{
	FString path = FString( TEXT( "/Game/GroundObject/" ) ) + InName;
	UClass* groundObject = LoadObject<UClass>( NULL, *path, NULL, LOAD_None, NULL );
	if( !groundObject )
		return nullptr;

	return SpawnActor( groundObject, InLocation, InRotator, nullptr );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 파티클 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::SpawnParticle( const FString& InEffectName, const AActor* InUseActor, const FVector& InLocation, const FRotator& InRotator )
{
	FString path = FString( TEXT( "/Game/Particle/" ) ) + InEffectName;
	UNiagaraSystem* effect = LoadObject<UNiagaraSystem>( NULL, *path, NULL, LOAD_None, NULL);
	if( !effect )
		return;

	UNiagaraFunctionLibrary::SpawnSystemAttached( effect, InUseActor->GetRootComponent(), NAME_None, InLocation, InRotator, EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 액터 제거 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::DestroyActor( AActor* InActor )
{
	auto gameObject = InActor ? Cast<UGameObject>( InActor->FindComponentByClass<UGameObject>() ) : nullptr;
	if( !gameObject )
		return;

	if( Objects.Find( gameObject->GetId() ) )
	{
		InActor->Destroy();
		Objects.Remove( gameObject->GetId() );

		if( AActorSpawner* spawner = SpawnerMap.FindRef( gameObject->GetId() ) )
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
//// @brief 리스트를 정리한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::Clear()
{
	Objects.Empty();
	SpawnerList.Empty();
	SpawnerMap.Empty();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스포너 등록 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::RegisterSpawner( AActorSpawner* InSpawner )
{
	if( !InSpawner )
		return;

	SpawnerList.Add( InSpawner );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스포너에서 스폰한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::SpawnActorInSpawner( float InDeltaTime )
{
	for( AActorSpawner* spawner : SpawnerList )
	{
		if( !spawner )
			continue;

		spawner->AddSpawnIntervalCount( InDeltaTime );

		if( spawner->CanSpawn() )
		{
			SpawnActor( spawner->GetActor(), spawner->GetActorLocation(), spawner->GetActorRotation(), spawner );

			spawner->AddSpawnCountTotal();
			spawner->AddSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
}
