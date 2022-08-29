#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

using namespace std;

class UMyGameInstance;

typedef TMap< EMaterialState, MaterialInfo > MaterialInfoMap;
typedef TMap< EWeaponState, WeaponInfo > WeaponInfoMap;

class DataInfoManager
{
private:
	MaterialInfoMap MaterialInfos;  // 물질 정보
	WeaponInfoMap   WeaponInfos;    // 무기 정보

public:
	DataInfoManager();
	~DataInfoManager();

	// 데이터 생성
	void DataCreate();

	// 틱 함수
	void Tick( float InDeltaTime );

	///////////////////////////////////////////////////////////////
	/// Get
	///////////////////////////////////////////////////////////////

	const MaterialInfoMap& GetMaterialInfos() { return MaterialInfos; };
	const WeaponInfoMap& GetWeaponInfos() { return WeaponInfos; };

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