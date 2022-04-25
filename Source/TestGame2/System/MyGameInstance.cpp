// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Manager/ObjectManager.h"


UMyGameInstance::UMyGameInstance()
{
	ObjectManager::CreateInstance();
}

UMyGameInstance::~UMyGameInstance()
{
	ObjectManager::DestroyInstance();
}