// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/CameraManager.h"
#include "../Manager/LockOnManager.h"
#include "../Manager/UIManager.h"
#include "../Actor/CharacterPC.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 에디터 시작시 O, 게임 시작 시 O
/////////////////////////////////////////////////////////////////////////////////////////////////////
UMyGameInstance::UMyGameInstance()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 에디터 종료시 O, 게임 종료 시 X
/////////////////////////////////////////////////////////////////////////////////////////////////////
UMyGameInstance::~UMyGameInstance()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 에디터 시작시 X, 게임 시작 시 O
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Init()
{
	Super::Init();

	ObjectManager::CreateInstance();
	DataInfoManager::CreateInstance();
	CameraManager::CreateInstance();
	LockOnManager::CreateInstance();
	UIManager::CreateInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 에디터 종료시 X, 게임 종료 시 O
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Shutdown()
{
	Super::Shutdown();

	ObjectManager::DestroyInstance();
	DataInfoManager::DestroyInstance();
	CameraManager::DestroyInstance();
	LockOnManager::DestroyInstance();
	UIManager::DestroyInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 틱 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Tick( float InDeltaTime )
{
	GetObjectManager().Tick( InDeltaTime );
	GetCameraManager().Tick( InDeltaTime );
	GetLockOnManager().Tick( InDeltaTime );
	GetUIManager().Tick( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 자신의 플레이어를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
ACharacterPC* UMyGameInstance::GetMyPlayer()
{
	UWorld* world = GetWorld();
	if( !world )
		return nullptr;

	APlayerController* controller = world->GetFirstPlayerController();
	if( !controller )
		return nullptr;

	ACharacterPC* player = Cast<ACharacterPC>( controller->GetPawn() );
	if( !player )
		return nullptr;

	return player;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 자신의 컨트롤러를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
APlayerController* UMyGameInstance::GetMyController()
{
	UWorld* world = GetWorld();
	if( !world )
		return nullptr;

	APlayerController* controller = world->GetFirstPlayerController();
	if( !controller )
		return nullptr;

	return controller;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 인스턴스를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
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
	//if( GEngine )
	//	GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, "game" );

	UGameEngine* GameEngine = Cast< UGameEngine >( GEngine );
	if( !GameEngine )
		return nullptr;

	return Cast< UMyGameInstance >( GameEngine->GameInstance );

#endif
}