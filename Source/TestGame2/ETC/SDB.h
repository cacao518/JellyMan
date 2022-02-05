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

    // Player
	constexpr float PLAYER_HP                    = 200.f;  // �÷��̾� ü��
	constexpr float PLAYER_MOVE_SPEED            = 1.0f;   // �÷��̾� �̵��ӵ�
	constexpr float PLAYER_ATTACK_SPEED          = 1.0f;   // �÷��̾� ���ݼӵ�

	// Dummy
	constexpr float DUMMY_HP           = 100.f;       // ���� ü��
	constexpr float DUMMY_MOVE_SPEED   = 0.6f;        // ���� �̵��ӵ�
	constexpr float DUMMY_ATTACK_SPEED = 1.0f;        // ���� ���ݼӵ�

};