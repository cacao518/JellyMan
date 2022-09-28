
#include "DataInfoManager.h"
#include "Engine/World.h"
#include "../System/MyGameInstance.h"
#include "../Component/GameObject.h"
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
	MaterialInfos.Add( EMaterialState::JELLY,  MaterialInfo{ 1.0f, 1.0f, 1.0f, 1.0f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor" } );
	MaterialInfos.Add( EMaterialState::GRASS,  MaterialInfo{ 1.3f, 1.1f, 1.3f, 0.3f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" } );
	MaterialInfos.Add( EMaterialState::GRAVEL, MaterialInfo{ 0.9f, 0.8f, 0.8f, 1.2f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel" } );
	MaterialInfos.Add( EMaterialState::ROCK,   MaterialInfo{ 0.4f, 0.5f, 0.5f, 3.0f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" } );
	MaterialInfos.Add( EMaterialState::WATER,  MaterialInfo{ 1.1f, 1.0f, 0.8f, 0.2f, 30.f, "Water", "/Water/Materials/WaterSurface/Water_FarMesh.Water_FarMesh" } );
	MaterialInfos.Add( EMaterialState::MARBLE, MaterialInfo{ 0.9f, 0.9f, 0.9f, 1.2f, 100.f, "Pawn", "/Game/Megascans/3D_Assets/Old_Decorative_Pillar_wjwhafddw/MI_Old_Decorative_Pillar_wjwhafddw_2K.MI_Old_Decorative_Pillar_wjwhafddw_2K" } );

	WeaponInfos.Add( EWeaponState::SWORD, WeaponInfo{ 5, 50.0f, "Sword" } );
}
