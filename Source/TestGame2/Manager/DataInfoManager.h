#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include <unordered_map>
#include <list>

using namespace std;

class UMyGameInstance;

// ���� ����
struct MaterialInfo
{
	float MoveSpeed;
	float AttackSpeed;
	float JumpPower;
	float Mass;
	FName CollisonName;
	FString AssetPath;
};

typedef unordered_map< EMaterialState, MaterialInfo > MaterialInfoList;

class DataInfoManager
{
private:
	MaterialInfoList MaterialInfos;

public:
	DataInfoManager();
	~DataInfoManager();

	// ������ ����
	void DataCreate();

	// ƽ �Լ�
	void Tick( float InDeltaTime );

	// ���� ���� ��ȯ
	const MaterialInfoList& GetMaterialInfos() { return MaterialInfos; };

	///////////////////////////////////////////////////////////////
	/// �̱��� �ڵ�
	///////////////////////////////////////////////////////////////
	static class DataInfoManager* Instance;
	static DataInfoManager& GetInstance(){ return *Instance; }
	static DataInfoManager* CreateInstance() { return !Instance ? Instance = new DataInfoManager() : Instance; }
	static void DestroyInstance() { if( !Instance ) delete Instance; Instance = nullptr; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief  LoadClass
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template< typename T >
	UClass* LoadClass( const FString& InPath )
	{
		FString copiedPath = InPath;
		return ConstructorHelpersInternal::FindOrLoadClass( copiedPath, T::StaticClass() );
	}
};
inline DataInfoManager& GetDataInfoManager() { return DataInfoManager::GetInstance(); };