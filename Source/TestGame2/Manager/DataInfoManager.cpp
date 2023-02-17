
#include "DataInfoManager.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "../System/MyGameInstance.h"
#include "GameFramework/Actor.h"


DataInfoManager::DataInfoManager()
{
	DataCreate();
}

DataInfoManager::~DataInfoManager()
{
	PlayerDefaultSkillInfos.Empty();
	PlayerWeaponSkillInfos.Empty();
	MaterialInfos.Empty();
	WeaponInfos.Empty();
	SkillInfos.Empty();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 데이터 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DataInfoManager::DataCreate()
{
	_LoadDataTable< FPlayerDefaultSkillInfo, PlayerDefaultSkillInfoMap >( PlayerDefaultSkillInfos, TEXT( "/Game/Data/DT_PlayerDefaultSkillInfo" ) );
	_LoadDataTable< FPlayerWeaponSkillInfo,  PlayerWeaponSkillInfoMap > ( PlayerWeaponSkillInfos,  TEXT( "/Game/Data/DT_PlayerWeaponSkillInfo" ) );
	_LoadDataTable< FMaterialInfo,           MaterialInfoMap >          ( MaterialInfos,           TEXT( "/Game/Data/DT_MaterialInfo" ) );
	_LoadDataTable< FWeaponInfo,             WeaponInfoMap >            ( WeaponInfos,             TEXT( "/Game/Data/DT_WeaponInfo" ) );
	_LoadDataTable< FSkillInfo,              SkillInfoMap >             ( SkillInfos,              TEXT( "/Game/Data/DT_SkillInfo" ) );
}
