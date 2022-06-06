// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Manager/ObjectManager.h"


UMyGameInstance::UMyGameInstance()
{
	ObjectManager::CreateInstance();
	GetObjectManager().SetGameInstance( this );
}

UMyGameInstance::~UMyGameInstance()
{
	ObjectManager::DestroyInstance();
}

void UMyGameInstance::Tick( float InDeltaTime )
{
	GetObjectManager().Tick( InDeltaTime );
}
