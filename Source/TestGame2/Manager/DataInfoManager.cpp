
#include "DataInfoManager.h"
#include "Engine/World.h"
#include "../System/MyGameInstance.h"
#include "../Component/GameObject.h"
#include "GameFramework/Actor.h"

DataInfoManager* DataInfoManager::Instance = nullptr;

DataInfoManager::DataInfoManager()
{
	DataCreate();
}

DataInfoManager::~DataInfoManager()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 데이터 생성
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DataInfoManager::DataCreate()
{
	MaterialInfos.Add( EMaterialState::JELLY,  MaterialInfo{ 1.0f, 1.0f, 1.0f, 1.0f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor" } );
	MaterialInfos.Add( EMaterialState::GRASS,  MaterialInfo{ 1.3f, 1.1f, 1.5f, 0.3f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" } );
	MaterialInfos.Add( EMaterialState::GRAVEL, MaterialInfo{ 0.9f, 0.8f, 0.8f, 1.2f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel" } );
	MaterialInfos.Add( EMaterialState::ROCK,   MaterialInfo{ 0.4f, 0.5f, 0.5f, 3.0f, 100.f, "Pawn", "/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" } );
	MaterialInfos.Add( EMaterialState::WATER,  MaterialInfo{ 1.1f, 1.0f, 0.7f, 0.2f, 30.f, "Water", "/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake" } );

	WeaponInfos.Add( EWeaponState::SWORD, WeaponInfo{ 50.0f, "Sword" } );
}
