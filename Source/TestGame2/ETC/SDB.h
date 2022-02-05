#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"

// 애니메이션 상태
UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN        UMETA( DisplayName = "Idle_Run" ),
	JUMP            UMETA( DisplayName = "Jump" ),
	DIE             UMETA( DisplayName = "Die" ),
	COMMON_ACTION   UMETA( DisplayName = "Common_Action" ),

	MAX,             
};

// 물질 상태
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

// 충돌체 정보
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
	constexpr float ANIM_LERP_MULITPLIER = 5.f;   // 애니메이션 중 이동 속도 보간 값
	constexpr float DEFAULT_MOVE_SPEED = 450.f;   // 기본 이동속도

    // Player
	constexpr float PLAYER_HP                    = 200.f;  // 플레이어 체력
	constexpr float PLAYER_MOVE_SPEED            = 1.0f;   // 플레이어 이동속도
	constexpr float PLAYER_ATTACK_SPEED          = 1.0f;   // 플레이어 공격속도

	// Dummy
	constexpr float DUMMY_HP           = 100.f;       // 더미 체력
	constexpr float DUMMY_MOVE_SPEED   = 0.6f;        // 더미 이동속도
	constexpr float DUMMY_ATTACK_SPEED = 1.0f;        // 더미 공격속도

};