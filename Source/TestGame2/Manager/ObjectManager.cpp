
#include "ObjectManager.h"
#include "Engine/World.h"
#include "../Actor/CharacterPC.h"
#include "../Actor/CharacterNPC.h"
#include "../Actor/StaticObject.h"
#include "../System/MyGameInstance.h"
#include "../Component/ObjectComp.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"


ObjectManager::ObjectManager()
{
	ObjectId = 1;
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
ActorPtr ObjectManager::SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator, ETeamType InTeamType, TWeakObjectPtr<AActorSpawner> InSpawner )
{
	const auto& actor = Objects.FindRef( ObjectId );
	if( actor.IsValid() )
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

		FTransform spawnTransform( InRotator, InLocation );
		AActor* newActor = World->SpawnActorDeferred< AActor >( InClass, spawnTransform );
		if( !newActor )
			return nullptr;

		auto objectComp = newActor ? Cast<UObjectComp>( newActor->FindComponentByClass<UObjectComp>() ) : nullptr;
		if( objectComp )
		{
			objectComp->SetId( ObjectId );
			objectComp->SetTeamType( InTeamType );
		}

		newActor->FinishSpawning( spawnTransform );

		if( InSpawner.IsValid() )
			SpawnerMap.Add( ObjectId, InSpawner );

		Objects.Add( ObjectId, newActor );

		ObjectId++;
		return newActor;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 정적 오브젝트 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
ActorPtr ObjectManager::SpawnStaticObject( const FString& InName, const FVector& InLocation, const FRotator& InRotator )
{
	FString path = FString( TEXT( "/Game/Blueprints/StaticObject/" ) ) + InName;
	UClass* staticObject = ConstructorHelpersInternal::FindOrLoadClass( path, AStaticObject::StaticClass() );
	if( !staticObject )
		return nullptr;

	return SpawnActor( staticObject, InLocation, InRotator, ETeamType::MAX );
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

	SpawnParticle( effect, InUseActor, InLocation, InRotator );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 파티클 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::SpawnParticle( UNiagaraSystem* InEffect, const AActor* InUseActor, const FVector& InLocation, const FRotator& InRotator )
{
	UNiagaraFunctionLibrary::SpawnSystemAttached( InEffect, InUseActor->GetRootComponent(), NAME_None, InLocation, InRotator, EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 액터 제거 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::DestroyActor( AActor* InActor )
{
	auto objectComp = InActor ? Cast<UObjectComp>( InActor->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !objectComp )
		return;

	if( Objects.Find( objectComp->GetId() ) )
	{
		InActor->Destroy();
		Objects.Remove( objectComp->GetId() );

		const auto& spawner = SpawnerMap.FindRef( objectComp->GetId() );
		if( spawner.IsValid() )
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
	for( const auto& spawner : SpawnerList )
	{
		if( !spawner.IsValid() )
			continue;

		spawner->AddSpawnIntervalCount( InDeltaTime );

		if( spawner->CanSpawn() )
		{
			SpawnActor( spawner->GetActorClass(), spawner->GetActorLocation(), spawner->GetActorRotation(), ETeamType::MAX, spawner );

			spawner->AddSpawnCountTotal();
			spawner->AddSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
}
