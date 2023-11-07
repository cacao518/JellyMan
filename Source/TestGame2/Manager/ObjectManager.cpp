
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
//// @brief ƽ �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::Tick( float InDeltaTime )
{
	SpawnActorInSpawner( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
AActor* ObjectManager::SpawnActor( UClass* InClass, const FVector& InLocation, const FRotator& InRotator, ETeamType InTeamType, AActorSpawner* InSpawner )
{
	const auto& actor = Objects.FindRef( ObjectId );
	if( actor.IsValid() )
	{
		actor->SetActorLocationAndRotation( InLocation, InRotator );
		return actor.Get();
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

		if( InSpawner )
			SpawnerMap.Add( ObjectId, ActorSpawnerPtr( InSpawner ) );

		Objects.Add( ObjectId, newActor );

		ObjectId++;

		newActor->FinishSpawning( spawnTransform );

		return newActor;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ������Ʈ ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
AActor* ObjectManager::SpawnStaticObject( const FString& InName, const FVector& InLocation, const FRotator& InRotator )
{
	FString path = FString( TEXT( "/Game/Blueprints/StaticObject/" ) ) + InName;
	UClass* staticObject = ConstructorHelpersInternal::FindOrLoadClass( path, AStaticObject::StaticClass() );
	if( !staticObject )
		return nullptr;

	return SpawnActor( staticObject, InLocation, InRotator, ETeamType::MAX );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ��ƼŬ ����
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
//// @brief ��ƼŬ ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::SpawnParticle( UNiagaraSystem* InEffect, const AActor* InUseActor, const FVector& InLocation, const FRotator& InRotator )
{
	UNiagaraFunctionLibrary::SpawnSystemAttached( InEffect, InUseActor->GetRootComponent(), NAME_None, InLocation, InRotator, EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ���� �Լ�
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
			spawner.Get()->SubSpawnCountInWorld();
			spawner.Get()->ResetSpawnIntervalCount();
		}
	}
	else
	{
		if( InActor )
			InActor->Destroy();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ����Ʈ�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::Clear()
{
	Objects.Empty();
	SpawnerList.Empty();
	SpawnerMap.Empty();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �����Ϳ��� ��ȯ�� ���͸� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::RegisterActorInEditor( AActor* InActor )
{
	auto objectComp = InActor ? Cast<UObjectComp>( InActor->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !objectComp )
		return;

	if( Objects.Find( objectComp->GetId() ) )
		return;

	objectComp->SetId( ObjectId );
	objectComp->SetTeamType( ETeamType::MAX );

	Objects.Add( ObjectId, InActor );

	ObjectId++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ��� �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectManager::RegisterSpawner( AActorSpawner* InSpawner )
{
	if( !InSpawner )
		return;

	SpawnerList.Add( InSpawner );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �����ʿ��� �����Ѵ�.
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
			SpawnActor( spawner->GetActorClass(), spawner->GetActorLocation(), spawner->GetActorRotation(), ETeamType::MAX, spawner.Get() );

			spawner->AddSpawnCountTotal();
			spawner->AddSpawnCountInWorld();
			spawner->ResetSpawnIntervalCount();
		}
	}
}
