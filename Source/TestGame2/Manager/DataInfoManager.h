#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

using namespace std;

class UMyGameInstance;

// 물질 정보
struct MaterialInfo
{
	float MoveSpeed;
	float AttackSpeed;
	float JumpPower;
	float Mass;
	FName CollisonName;
	FString AssetPath;
};

typedef unordered_map< EMaterialState, MaterialInfo > MaterialInfoList;

class DataInfoManager
{
public:
	MaterialInfoList MaterialInfos;

public:
	DataInfoManager();
	~DataInfoManager();

	// 데이터 생성
	void DataCreate();

	// 틱 함수
	void Tick( float InDeltaTime );

	///////////////////////////////////////////////////////////////
	/// 싱글톤 코드
	///////////////////////////////////////////////////////////////
	static class DataInfoManager* Instance;
	static DataInfoManager& GetInstance(){ return *Instance; }
	static DataInfoManager* CreateInstance() { return !Instance ? Instance = new DataInfoManager() : Instance; }
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
inline DataInfoManager& GetDataInfoManager() { return DataInfoManager::GetInstance(); };