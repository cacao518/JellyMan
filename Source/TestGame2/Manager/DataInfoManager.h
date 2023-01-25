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
	MaterialInfoMap MaterialInfos;  // ���� ����
	WeaponInfoMap   WeaponInfos;    // ���� ����
	SkillInfoMap    SkillInfos;     // ��ų ����

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
	const SkillInfoMap& GetSkillInfos() { return SkillInfos; };

private:
	//  ������ ���̺��� ���� �ʿ� �ҷ��´�.
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