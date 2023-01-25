#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


using MaterialInfoMap = TMap< EMaterialState, FMaterialInfo >;
using WeaponInfoMap   = TMap< EWeaponState, FWeaponInfo >;
using SkillInfoMap    = TMap< int, FSkillInfo >;


class DataInfoManager final : public SingletonBase< DataInfoManager >
{
private:
	MaterialInfoMap MaterialInfos;  // 물질 정보
	WeaponInfoMap   WeaponInfos;    // 무기 정보
	SkillInfoMap    SkillInfos;     // 스킬 정보

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
	const SkillInfoMap& GetSkillInfos() { return SkillInfos; };

private:
	//  데이터 테이블을 인포 맵에 불러온다.
	template<typename T1, typename T2>
	void _LoadDataTable( T2& InInfoMap, FString InPath )
	{
		if( UDataTable* dt = LoadObject<UDataTable>( NULL, *InPath, NULL, LOAD_None, NULL ) )
		{
			for( auto& [_, value] : dt->GetRowMap() )
			{
				T1* row = (T1*)value;
				InInfoMap.Add( row->GetKey(), *row );
			}
		}
	}
};
inline DataInfoManager& GetDataInfoManager() { return DataInfoManager::GetInstance(); };