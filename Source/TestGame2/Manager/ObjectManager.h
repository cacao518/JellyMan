// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	///////////////////////////////////////////////////////////////
	/// ½Ì±ÛÅæ ÄÚµå
	///////////////////////////////////////////////////////////////
	static class ObjectManager* Instance;
	static ObjectManager& GetInstance(){ return *Instance; }
	static ObjectManager* CreateInstance() { return !Instance ? Instance = new ObjectManager() : Instance; }
	static void DestroyInstance() { if( !Instance ) delete Instance; Instance = nullptr; }
};
inline ObjectManager& GetObjectManager() { ObjectManager::GetInstance(); };