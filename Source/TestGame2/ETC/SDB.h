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

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER = 10.0f;          // 애니메이션 중 이동 속도 보간 값

    // Player
	constexpr float PLAYER_DEFAULT_SPEED          = 450.f; // 플레이어 기본 이동속도
	constexpr float PLAYER_ROLL_MOVE_MULITPLIER   = 20.0f; // 플레이어 구르기 이동 곱셈 값
	constexpr float PLAYER_PUNCH1_MOVE_MULITPLIER = 1.1f;  // 플레이어 펀지1 이동 곱셈 값
	constexpr float PLAYER_PUNCH2_MOVE_MULITPLIER = 4.0f;  // 플레이어 펀치2 이동 곱셈 값

	// Player Skill
	static const FVector PLAYER_PUNCH1_ATTACK_SIZE = FVector( 85, 133, 80 );  // 플레이어 펀치1 판정 크기
	static const FVector PLAYER_PUNCH1_ATTACK_POS = FVector( 0, -191, 364 );  // 플레이어 펀치1 판정 위치

    static const FVector PLAYER_PUNCH2_ATTACK_SIZE = FVector( 90, 201, 236 );  // 플레이어 펀치2 판정 크기
	static const FVector PLAYER_PUNCH2_ATTACK_POS = FVector( 0, -192, 392 );  // 플레이어 펀치1 판정 위치
};