#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"

// ������Ʈ ����
UENUM( BlueprintType )
enum class EObjectType : uint8
{
	PC         UMETA( DisplayName = "PC" ),
	NPC        UMETA( DisplayName = "NPC" ),
	PROJECTILE UMETA( DisplayName = "PROJECTILE" ),
	OBJECT     UMETA( DisplayName = "OBJECT" ),
	MAX,
};

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

// ��ų ����
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

// ���� ����
USTRUCT( Atomic, BlueprintType )
struct FStatusInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Level; 	// ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Hp; 	// ü��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Hpm; 	// ü��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MoveSpeed;	// �̵��ӵ�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackSpeed;	// ���ݼӵ�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackPower;	// ���ݷ�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float DefensePower;	// ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float JumpPower;	// ������

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Strength;   	// ������ MAX:100( �����ð�, �˹�Ÿ� ���� ) 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Weight;   	// ���� MAX:100 ( ����, ��� ���� )
};

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER = 5.f;   // �ִϸ��̼� �� �̵� �ӵ� ���� ��
	constexpr float DEFAULT_MOVE_SPEED = 450.f;   // �⺻ �̵��ӵ�
	constexpr float DEFAULT_JUMP_POWER = 600.f;   // �⺻ ������

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