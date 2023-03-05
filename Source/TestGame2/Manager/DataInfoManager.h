#pragma once

#include "../ETC/SDB.h"
#include "SingletonBase.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>


using PlayerDefaultSkillInfoMap = TMap< EInputKeyType,  FPlayerDefaultSkillInfo >;
using PlayerWeaponSkillInfoMap  = TMap< EWeaponState,   FPlayerWeaponSkillInfo >;
using MaterialInfoMap           = TMap< EMaterialState, FMaterialInfo >;
using WeaponInfoMap             = TMap< int,            FWeaponInfo >;
using SkillInfoMap              = TMap< int,            FSkillInfo >;


class DataInfoManager final : public SingletonBase< DataInfoManager >
{
private:
	PlayerDefaultSkillInfoMap PlayerDefaultSkillInfos;  // �÷��̾� �⺻ ��ų ����
	PlayerWeaponSkillInfoMap  PlayerWeaponSkillInfos;   // �÷��̾� ���� ��ų ����

	MaterialInfoMap           MaterialInfos;            // ���� ����
	WeaponInfoMap             WeaponInfos;              // ���� ����
	SkillInfoMap              SkillInfos;               // ��ų ����

public:
	DataInfoManager();
	~DataInfoManager();

	// ������ ����
	void DataCreate();

	///////////////////////////////////////////////////////////////
	/// Get
	///////////////////////////////////////////////////////////////

	const PlayerDefaultSkillInfoMap& GetPlayerDefaultSkillInfos() { return PlayerDefaultSkillInfos; };
	const PlayerWeaponSkillInfoMap& GetPlayerWeaponSkillInfos() { return PlayerWeaponSkillInfos; };
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