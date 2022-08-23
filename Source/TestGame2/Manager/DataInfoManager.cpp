
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
//// @brief 单捞磐 积己
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DataInfoManager::DataCreate()
{
	MaterialInfos[ EMaterialState::JELLY ] = MaterialInfo{ 1.f, 1.f, 1.f, 1.f, "Pawn", "/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor" };
	MaterialInfos[ EMaterialState::GRASS ] = MaterialInfo{ 1.3f, 1.1f, 1.5f, 0.3f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss" };
	MaterialInfos[ EMaterialState::GRAVEL ] = MaterialInfo{ 0.9f, 0.8f, 0.8f, 1.2f, "Pawn", "/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel" };
	MaterialInfos[ EMaterialState::ROCK ] = MaterialInfo{ 0.4f, 0.5f, 0.5f, 3.0f, "Pawn", "/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt" };
	MaterialInfos[ EMaterialState::WATER ] = MaterialInfo{ 1.1f, 1.f, 0.7f, 0.2f, "Water", "/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake" };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 平 窃荐
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DataInfoManager::Tick( float InDeltaTime )
{

}