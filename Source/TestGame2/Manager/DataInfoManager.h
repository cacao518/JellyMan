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
	MaterialInfoMap MaterialInfos;  // 물질 정보
	WeaponInfoMap   WeaponInfos;    // 무기 정보

public:
	DataInfoManager();
	~DataInfoManager();

	// 데이터 생성
	void DataCreate();

	///////////////////////////////////////////////////////////////
	/// Get
	///////////////////////////////////////////////////////////////

	const MaterialInfoMap& GetMaterialInfos() { return MaterialInfos; };
	const WeaponInfoMap& GetWeaponInfos() { return WeaponInfos; };

};
inline DataInfoManager& GetDataInfoManager() { return DataInfoManager::GetInstance(); };