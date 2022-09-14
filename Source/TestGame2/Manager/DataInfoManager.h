#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

using namespace std;

class UMyGameInstance;

using MaterialInfoMap = TMap< EMaterialState, MaterialInfo >;
using WeaponInfoMap   = TMap< EWeaponState, WeaponInfo >;

class DataInfoManager
{
private:
	MaterialInfoMap MaterialInfos;  // ���� ����
	WeaponInfoMap   WeaponInfos;    // ���� ����

public:
	DataInfoManager();
	~DataInfoManager();

	// ������ ����
	void DataCreate();

	///////////////////////////////////////////////////////////////
	/// Get
	///////////////////////////////////////////////////////////////

	const MaterialInfoMap& GetMaterialInfos() { return MaterialInfos; };
	const WeaponInfoMap& GetWeaponInfos() { return WeaponInfos; };

	///////////////////////////////////////////////////////////////
	/// �̱��� �ڵ�
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