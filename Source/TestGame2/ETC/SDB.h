#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"

// �ִϸ��̼� ����
UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN        UMETA( DisplayName = "Idle_Run" ),
	JUMP            UMETA( DisplayName = "Jump" ),
	DIE             UMETA( DisplayName = "Die" ),
	COMMON_ACTION   UMETA( DisplayName = "Common_Action" ),

	MAX,             
};

// ���� ����
UENUM( BlueprintType )
enum class EMaterialState : uint8
{
	JELLY            UMETA( DisplayName = "Jelly" ),
	GRASS            UMETA( DisplayName = "Grass" ),
	ROCK             UMETA( DisplayName = "Rock" ),
	GRAVEL           UMETA( DisplayName = "Gravel" ),
	WATER            UMETA( DisplayName = "Water" ),

	MAX,
};

// ���� ����
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	SWORD            UMETA( DisplayName = "Sword" ),

	MAX,
};

// �浹ü ����
USTRUCT( Atomic, BlueprintType )
struct FCollisionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FVector Size;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FVector Pos;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Power;

	FCollisionInfo()
	{
		Size = FVector(0,0,0);
		Pos = FVector(0,0,0);
		Power = 0;
	};

	FCollisionInfo( FVector InSize, FVector InPos, float InPower )
	{
		Size = InSize;
		Pos = InPos;
		Power = InPower;
	};
};

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER = 5.f;   // �ִϸ��̼� �� �̵� �ӵ� ���� ��
	constexpr float DEFAULT_MOVE_SPEED = 450.f;   // �⺻ �̵��ӵ�
	constexpr float DEFAULT_JUMP_POWER = 600.f;   // �⺻ ������

    // Player
	constexpr float PLAYER_HP                    = 200.f;  // �÷��̾� ü��
	constexpr float PLAYER_MOVE_SPEED            = 1.f;   // �÷��̾� �̵��ӵ�
	constexpr float PLAYER_ATTACK_SPEED          = 1.f;   // �÷��̾� ���ݼӵ�
	constexpr float PLAYER_JUMP_POWER            = 1.f;   // �÷��̾� ������

	// Dummy
	constexpr float DUMMY_HP           = 100.f;       // ���� ü��
	constexpr float DUMMY_MOVE_SPEED   = 0.6f;        // ���� �̵��ӵ�
	constexpr float DUMMY_ATTACK_SPEED = 1.f;        // ���� ���ݼӵ�
	constexpr float DUMMY_JUMP_POWER   = 1.f;        // ���� ������


	// Material 
	constexpr float GRASS_MOVE_SPEED   = 1.1f;   
	constexpr float GRASS_ATTACK_SPEED = 1.1f;   
	constexpr float GRASS_JUMP_POWER   = 1.1f;  

	constexpr float ROCK_MOVE_SPEED   = 0.4f;   
	constexpr float ROCK_ATTACK_SPEED = 0.5f;  
	constexpr float ROCK_JUMP_POWER   = 0.5f;   

	constexpr float GRAVEL_MOVE_SPEED = 0.7f;   
	constexpr float GRAVEL_ATTACK_SPEED = 0.8f;  
	constexpr float GRAVEL_JUMP_POWER = 0.7f;  

	constexpr float WATER_MOVE_SPEED = 1.1f;   
	constexpr float WATER_ATTACK_SPEED = 1.0f; 
	constexpr float WATER_JUMP_POWER = 0.8f;  
};