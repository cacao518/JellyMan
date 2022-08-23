// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ���۽� O, ���� ���� �� O
/////////////////////////////////////////////////////////////////////////////////////////////////////
UMyGameInstance::UMyGameInstance()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ����� O, ���� ���� �� X
/////////////////////////////////////////////////////////////////////////////////////////////////////
UMyGameInstance::~UMyGameInstance()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ���۽� X, ���� ���� �� O
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Init()
{
	Super::Init();

	ObjectManager::CreateInstance();
	DataInfoManager::CreateInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ����� X, ���� ���� �� O
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Shutdown()
{
	Super::Shutdown();

	ObjectManager::DestroyInstance();
	DataInfoManager::DestroyInstance();

	GEngine->Exec( GWorld, TEXT( "Slate.SkipSecondPrepass 0" ) );
}

void UMyGameInstance::Tick( float InDeltaTime )
{
	GetObjectManager().Tick( InDeltaTime );
	GetDataInfoManager().Tick( InDeltaTime );
}

UMyGameInstance* UMyGameInstance::GetInstance()
{
	if( !GEngine ) 
		return nullptr;
	
#if WITH_EDITOR

	for( const FWorldContext& worldContext : GEngine->GetWorldContexts() )
	{
		if( worldContext.WorldType == EWorldType::PIE || 
			worldContext.WorldType == EWorldType::Game )
			return Cast< UMyGameInstance >( worldContext.OwningGameInstance );
	}
	return nullptr;

#else
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, "game" );

	UGameEngine* GameEngine = Cast< UGameEngine >( GEngine );
	if( !GameEngine )
		return nullptr;

	return Cast< UMyGameInstance >( GameEngine->GameInstance );

#endif
}