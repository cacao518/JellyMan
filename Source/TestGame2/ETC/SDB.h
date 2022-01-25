#pragma once

#include "CoreMinimal.h"

UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN        UMETA( DisplayName = "Idle_Run" ),
	JUMP            UMETA( DisplayName = "Jump" ),
	DIE             UMETA( DisplayName = "Die" ),
	ROLL            UMETA( DisplayName = "Roll" ),
	PUNCH1          UMETA( DisplayName = "Punch1" ),
	PUNCH2          UMETA( DisplayName = "Punch2" ),

	MAX,
};

struct CollisionInfo
{
public:
	FVector Size;
	FVector Pos;
	float Power;

	CollisionInfo()
	{
		Size = FVector(0,0,0);
		Pos = FVector(0,0,0);
		Power = 0;
	};

	CollisionInfo( FVector InSize, FVector InPos, float InPower )
	{
		Size = InSize;
		Pos = InPos;
		Power = InPower;
	};
};

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER = 10.0f;          // 애니메이션 중 이동 속도 보간 값

    // Player
	constexpr float PLAYER_HP                     = 200.f; // 플레이어 체력
	constexpr float PLAYER_DEFAULT_SPEED          = 450.f; // 플레이어 이동속도
	constexpr float PLAYER_ROLL_MOVE_MULITPLIER   = 20.0f; // 플레이어 구르기 이동 곱셈 값
	constexpr float PLAYER_PUNCH1_MOVE_MULITPLIER = 1.1f;  // 플레이어 펀지1 이동 곱셈 값
	constexpr float PLAYER_PUNCH2_MOVE_MULITPLIER = 4.0f;  // 플레이어 펀치2 이동 곱셈 값

	// Player Skill
	static const CollisionInfo PLAYER_PUNCH1_COLLISION_INFO = CollisionInfo ( FVector( 85, 133, 80 ),
	                                                                          FVector( 0, -191, 364 ),
																		      5.f );   // 플레이어 펀치1

	static const CollisionInfo PLAYER_PUNCH2_COLLISION_INFO = CollisionInfo( FVector( 90, 201, 236 ),
																			 FVector( 0, -192, 392 ),
																			 15.f );   // 플레이어 펀치2

	// Dummy
	constexpr float DUMMY_HP =    100.f;       // 더미 체력
	constexpr float DUMMY_SPEED = 100.f;       // 더미 이동속도

};