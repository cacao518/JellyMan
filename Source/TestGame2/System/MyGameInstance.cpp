// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/CameraManager.h"
#include "../Manager/LockOnManager.h"
#include "../Manager/UIManager.h"
#include "../Character/CharacterPC.h"


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
	CameraManager::CreateInstance();
	LockOnManager::CreateInstance();
	UIManager::CreateInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ ����� X, ���� ���� �� O
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Shutdown()
{
	Super::Shutdown();

	ObjectManager::DestroyInstance();
	DataInfoManager::DestroyInstance();
	CameraManager::DestroyInstance();
	LockOnManager::DestroyInstance();
	UIManager::CreateInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ƽ �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UMyGameInstance::Tick( float InDeltaTime )
{
	GetObjectManager().Tick( InDeltaTime );
	GetCameraManager().Tick( InDeltaTime );
	GetLockOnManager().Tick( InDeltaTime );
	GetUIManager().Tick( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ڽ��� �÷��̾ ��ȯ�Ѵ�.
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
//// @brief �ڽ��� ��Ʈ�ѷ��� ��ȯ�Ѵ�.
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
//// @brief �ν��Ͻ��� ��ȯ�Ѵ�.
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
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, "game" );

	UGameEngine* GameEngine = Cast< UGameEngine >( GEngine );
	if( !GameEngine )
		return nullptr;

	return Cast< UMyGameInstance >( GameEngine->GameInstance );

#endif
}