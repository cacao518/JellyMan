#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


using MaterialInfoMap = TMap< EMaterialState, MaterialInfo >;
using WeaponInfoMap   = TMap< EWeaponState, WeaponInfo >;


class DataInfoManager final : public SingletonBase< DataInfoManager >
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

};
inline DataInfoManager& GetDataInfoManager() { return DataInfoManager::GetInstance(); };