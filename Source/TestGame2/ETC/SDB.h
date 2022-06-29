#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"

// 오브젝트 종류
UENUM( BlueprintType )
enum class EObjectType : uint8
{
	PC         UMETA( DisplayName = "PC" ),
	NPC        UMETA( DisplayName = "NPC" ),
	PROJECTILE UMETA( DisplayName = "PROJECTILE" ),
	OBJECT     UMETA( DisplayName = "OBJECT" ),
	MAX,
};

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

// 무기 상태
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	SWORD            UMETA( DisplayName = "Sword" ),

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

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float KnockBackPower;

	FCollisionInfo()
	{
		Size = FVector(0,0,0);
		Pos = FVector(0,0,0);
		Power = 0;
	};

	FCollisionInfo( FVector InSize, FVector InPos, float InPower, float InKnockBackPower )
	{
		Size = InSize;
		Pos = InPos;
		Power = InPower;
		KnockBackPower = InKnockBackPower;
	};
};

// 스킬 정보
USTRUCT( Atomic, BlueprintType )
struct FSkillInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	UAnimMontage* Anim;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Num;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMin;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMax;
};

// 스텟 정보
USTRUCT( Atomic, BlueprintType )
struct FStatusInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Level; 	// 레벨

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Hp; 	// 체력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Hpm; 	// 체력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MoveSpeed;	// 이동속도

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackSpeed;	// 공격속도

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackPower;	// 공격력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float DefensePower;	// 방어력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float JumpPower;	// 점프력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Strength;   	// 강인함 MAX:100( 경직시간, 넉백거리 관련 ) 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Weight;   	// 무게 MAX:100 ( 점프, 에어본 관련 )
};

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER = 5.f;   // 애니메이션 중 이동 속도 보간 값
	constexpr float DEFAULT_MOVE_SPEED = 450.f;   // 기본 이동속도
	constexpr float DEFAULT_JUMP_POWER = 600.f;   // 기본 점프력

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
	constexpr float WATER_JUMP_POWER = 0.7f;  
};