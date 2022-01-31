#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"

UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN        UMETA( DisplayName = "Idle_Run" ),
	JUMP            UMETA( DisplayName = "Jump" ),
	DIE             UMETA( DisplayName = "Die" ),
	COMMON_ACTION   UMETA( DisplayName = "Common_Action" ),

	MAX,
};

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

    // Player
	constexpr float PLAYER_HP                            = 200.f; // 플레이어 체력
	constexpr float PLAYER_DEFAULT_MOVE_SPEED            = 450.f; // 플레이어 이동속도
	constexpr float PLAYER_DEFAULT_ATTACK_SPEED          = 1.0f;   // 플레이어 공격속도

	// Dummy
	constexpr float DUMMY_HP           = 100.f;        // 더미 체력
	constexpr float DUMMY_MOVE_SPEED   = 100.f;        // 더미 이동속도
	constexpr float DUMMY_ATTACK_SPEED = 1.f;          // 더미 공격속도

};