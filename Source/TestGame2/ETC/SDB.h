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
	constexpr float ANIM_LERP_MULITPLIER = 10.0f;

    // Player
	constexpr float PLAYER_DEFAULT_SPEED          = 450.f;
	constexpr float PLAYER_ROLL_MOVE_MULITPLIER   = 20.0f;
	constexpr float PLAYER_PUNCH1_MOVE_MULITPLIER = 1.1f;
	constexpr float PLAYER_PUNCH2_MOVE_MULITPLIER = 4.0f;

	// Player Skill
	static const FVector PLAYER_PUNCH1_ATTACK_SIZE = FVector( 50, 50, 50 );
    static const FVector PLAYER_PUNCH2_ATTACK_SIZE = FVector( 50, 50, 50 );
};