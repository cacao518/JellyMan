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
	constexpr float ANIM_LERP_MULITPLIER = 10.0f;          // �ִϸ��̼� �� �̵� �ӵ� ���� ��

    // Player
	constexpr float PLAYER_HP                     = 200.f; // �÷��̾� ü��
	constexpr float PLAYER_DEFAULT_SPEED          = 450.f; // �÷��̾� �̵��ӵ�
	constexpr float PLAYER_ROLL_MOVE_MULITPLIER   = 20.0f; // �÷��̾� ������ �̵� ���� ��
	constexpr float PLAYER_PUNCH1_MOVE_MULITPLIER = 1.1f;  // �÷��̾� ����1 �̵� ���� ��
	constexpr float PLAYER_PUNCH2_MOVE_MULITPLIER = 4.0f;  // �÷��̾� ��ġ2 �̵� ���� ��

	// Player Skill
	static const CollisionInfo PLAYER_PUNCH1_COLLISION_INFO = CollisionInfo ( FVector( 85, 133, 80 ),
	                                                                          FVector( 0, -191, 364 ),
																		      5.f );   // �÷��̾� ��ġ1

	static const CollisionInfo PLAYER_PUNCH2_COLLISION_INFO = CollisionInfo( FVector( 90, 201, 236 ),
																			 FVector( 0, -192, 392 ),
																			 15.f );   // �÷��̾� ��ġ2

	// Dummy
	constexpr float DUMMY_HP =    100.f;       // ���� ü��
	constexpr float DUMMY_SPEED = 100.f;       // ���� �̵��ӵ�

};