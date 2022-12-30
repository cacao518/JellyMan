
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
	MaterialInfos.Empty();
	WeaponInfos.Empty();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 데이터 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DataInfoManager::DataCreate()
{
	_LoadDataTable< FMaterialInfo, MaterialInfoMap >( MaterialInfos, TEXT( "/Game/Data/DT_MaterialInfo" ) );
	_LoadDataTable< FWeaponInfo,   WeaponInfoMap >  ( WeaponInfos,   TEXT( "/Game/Data/DT_WeaponInfo" ) );
}
