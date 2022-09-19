// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"

template< typename Type >
class SingletonBase
{
private:
	static Type* Instance;

public:
	SingletonBase(){ Instance = static_cast< Type* >( this ); };
	~SingletonBase(){ Instance = nullptr; };
	static Type& GetInstance(){ return *Instance; }
	static Type* CreateInstance() { return !Instance ? Instance = new Type() : Instance; }
	static void DestroyInstance() { if( !Instance ) delete Instance; Instance = nullptr; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief  LoadClass
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template< typename T >
	UClass* LoadClass( const FString& InPath )
	{
		FString copiedPath = InPath;
		return ConstructorHelpersInternal::FindOrLoadClass( copiedPath, T::StaticClass() );
	}
};

template< typename Type >
Type* SingletonBase< Type >::Instance = nullptr;