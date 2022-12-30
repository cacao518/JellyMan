#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SDB.generated.h"


using namespace std;


// ������Ʈ ����
UENUM( BlueprintType )
enum class EObjectType : uint8
{
	PC				  UMETA( DisplayName = "PC" ),
	NPC				  UMETA( DisplayName = "NPC" ),
	PROJECTILE        UMETA( DisplayName = "PROJECTILE" ),
	STATIC_OBJECT     UMETA( DisplayName = "STATIC_OBJECT" ),
	MAX,
};

// �ִϸ��̼� ����
UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN            UMETA( DisplayName = "Idle_Run" ),
	JUMP                UMETA( DisplayName = "Jump" ),
	DIE                 UMETA( DisplayName = "Die" ),
	COMMON_ACTION       UMETA( DisplayName = "Common_Action" ),

	MAX,             
};

// �ִϸ��̼� ���� ����
UENUM( BlueprintType )
enum class EAnimSubState : uint8
{
	NONE                UMETA( DisplayName = "None" ),
	MOVABLE             UMETA( DisplayName = "Movable" ),
	UPPER_LOWER_BLEND   UMETA( DisplayName = "Upper_Lower_Blend" ),

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
	MARBLE           UMETA( DisplayName = "Marble" ),

	MAX,
};

// ���� ����
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	SWORD            UMETA( DisplayName = "Sword" ),

	MAX,
};

// �Է� Ű ����
UENUM( BlueprintType )
enum class EInputKeyType : uint8
{
	LEFT_MOUSE            UMETA( DisplayName = "LeftMouse" ),
	RIGHT_MOUSE           UMETA( DisplayName = "RightMouse" ),
	BOTH_MOUSE            UMETA( DisplayName = "BothMouse" ),
	SPACE                 UMETA( DisplayName = "Space" ),

	MAX,
};

// ��ų �÷��� ����ӵ� ����
UENUM( BlueprintType )
enum class ESkillPlaySpeedType : uint8
{
	DEFAULT              UMETA( DisplayName = "Default" ),
	ATTACK_SPEED         UMETA( DisplayName = "AttackSpeed" ),
	MOVE_SPEED           UMETA( DisplayName = "MoveSpeed" ),

	MAX,
};

// �� ����
UENUM( BlueprintType )
enum class ETeamType : uint8
{
	NEUTRAL              UMETA( DisplayName = "Neutral" ),
	A                    UMETA( DisplayName = "A" ),
	B                    UMETA( DisplayName = "B" ),
	C                    UMETA( DisplayName = "C" ),
	D                    UMETA( DisplayName = "D" ),

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
	UAnimMontage* Anim;                                // ��Ÿ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Num;                                           // �ĺ���

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;                                    // ��Ÿ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	ESkillPlaySpeedType PlaySpeedType;                 // ��ų ����ӵ� Ÿ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EAnimSubState AnimSubState;                        // �ִϸ��̼� ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<EAnimState> PlayEnableState;                // ��ų ��� ������ ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<int> PlayEnableSkillNum;                    // ��ų ��� ������ ��ų���̵�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int DerivedSkillNum;                               // �Ļ� ��ų �ĺ��� 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	bool LockOnLookAt;                                 // ���� ���¿��� ��ų ���� �� �ٶ� ������ ���� 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMin;                            // ��ų �ߵ� ���� �ּ� (���� ����)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMax;                            // ��ų �ߵ� ���� �ִ� (���� ����)
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
	float Weight;   	// ����
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// DataInfo
/////////////////////////////////////////////////////////////////////////////////////////////////////

// ���� ����
USTRUCT( Atomic, BlueprintType )
struct FMaterialInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EMaterialState State;    // ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Mass;              // ����(���ݼӵ�,�̵��ӵ�,������ ����)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Intensity;         // ����(���ݷ�,����,������ ����, ���ݸ��)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MatEnergyMax;      // �ִ� ���� ������

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName CollisonName;      // ���� ���� �� �ݸ����̸�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString AssetPath;       // �ּ� ���

	/// �� Ű�� ��ȯ�Ѵ�.
	EMaterialState GetKey(){ return State; };
};

// ���� ����
USTRUCT( Atomic, BlueprintType )
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EWeaponState State;         // ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int   DurabilityMax;        // ���� ������ �ִ�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float RequireJellyAmount;   // ���⸦ �����ϴµ� �ʿ��� ���� ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ComponentName;        // ����ƽ �޽� ������Ʈ �̸�

	/// �� Ű�� ��ȯ�Ѵ�.
	EWeaponState GetKey(){ return State; };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// Const
/////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER      = 5.f;       // �ִϸ��̼� �� �̵� �ӵ� ���� ��
	constexpr float DEFAULT_MOVE_SPEED        = 450.f;     // �⺻ �̵��ӵ�
	constexpr float DEFAULT_JUMP_POWER        = 750.f;     // �⺻ ������
	constexpr float DEFAULT_ROTATION_RATE     = 1000.f;    // �⺻ ĳ���� ���� ��ȯ ����
	constexpr float FLY_ROTATION_RATE         = 100.f;     // ���� ĳ���� ���� ��ȯ ����
	constexpr float READY_SKILL_ROTATION_RATE = 100000.f;  // �ߵ� ��� ��ų ���� ĳ���� ���� ��ȯ ����
	constexpr float READY_SKILL_DURATION      = 0.4f;      // �ߵ� ������� ��ų ���� ���ӽð�
	constexpr float MONTAGE_INITIAL_TIME      = 0.05f;     // ��Ÿ�� �ߵ� �� �ʱ�ð� ( ������ȯ���� �� LookAt �ð�)
	constexpr float DEAD_ACTOR_DESTROY_TIME   = 3.f;       // ��� �� �ش� �� ���� ���� ����

	constexpr float DEFAULT_TARGET_ARM_LENGTH = 280.f;     // �⺻ ī�޶� �� ����
	constexpr float FLY_TARGET_ARM_LENGTH     = 400.f;     // ���� ī�޶� �� ����

	constexpr float DEFAULT_GRAVITY_SCALE     = 2.0f;      // �⺻ �߷�
	constexpr float FLY_GRAVITY_SCALE         = 0.3f;      // ���� �߷�

	constexpr float LOCKON_RANGE = 800.f;                  // ���� ����
	constexpr float LOCKON_CAMERA_ROTAION_LAG_SPEED = 5.f; // ���� ī�޶� ȸ�� ������ �ӵ�

	constexpr float MAX_MASS                  = 2.f;       // �ִ� ����
	constexpr float MAX_INTENSITY             = 2.f;       // �ִ� ����

	constexpr float MIDDLE_RATE              = 1.2f;      // �߰� ����/���� ����
	constexpr float HARD_RATE                = 1.5f;      // �� ����/���� ����
};