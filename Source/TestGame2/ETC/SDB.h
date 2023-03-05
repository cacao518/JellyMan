#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
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
	DEFAULT             UMETA( DisplayName = "Default" ),
	MOVABLE             UMETA( DisplayName = "Movable" ),
	UPPER_LOWER_BLEND   UMETA( DisplayName = "Upper_Lower_Blend" ),

	MAX,
};

// ���� ����
UENUM( BlueprintType )
enum class EMaterialState : uint8
{
	DEFAULT          UMETA( DisplayName = "Default" ),
	JELLY            UMETA( DisplayName = "Jelly" ),
	GRASS            UMETA( DisplayName = "Grass" ),
	ROCK             UMETA( DisplayName = "Rock" ),
	GRAVEL           UMETA( DisplayName = "Gravel" ),
	WATER            UMETA( DisplayName = "Water" ),
	DEEPWATER        UMETA( DisplayName = "DeepWater" ),
	MARBLE           UMETA( DisplayName = "Marble" ),

	MAX,
};

// ���� ����
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	DEFAULT          UMETA( DisplayName = "Default" ),
	SWORD            UMETA( DisplayName = "Sword" ),
	AXE              UMETA( DisplayName = "Axe" ),
	SPEAR            UMETA( DisplayName = "Spear" ),

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
	SHIFT                 UMETA( DisplayName = "Shift" ),
	Tab                   UMETA( DisplayName = "Tab" ),
	F                     UMETA( DisplayName = "F" ),
	R                     UMETA( DisplayName = "R" ),
	Num1                  UMETA( DisplayName = "Num1" ),
	Num2                  UMETA( DisplayName = "Num2" ),
	Num3                  UMETA( DisplayName = "Num3" ),

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
	float Mp; 	// ���¹̳�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Mpm; 	// ���¹̳�

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

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	bool IsTyrant;      // ���� ���� (�� ��� ���� Ÿ��)
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
	EMaterialState State;                            // ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Mass;                                      // ����(���ݼӵ�,�̵��ӵ�,������ ����)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Intensity;                                 // ����(���ݷ�,����,������ ���� )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MatEnergyMax;                              // �ִ� ���� ������

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName CollisonName;                              // ���� ���� �� �ݸ����̸�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray < FString > MaterialAssetPaths;         // ���͸��� �ּ°�� ( �ش� �ϴ� ���͸����� State�� �� �� �ִ� )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString FootStepParticleName;                // ���� ���� �� ������ ��ƼŬ �̸�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString ChangeParticleName;                  // �ش� ������ ����Ǿ��� �� ������ ��ƼŬ �̸�

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
	int Num;                    // �ĺ���

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EWeaponState State;         // ���� ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int   DurabilityMax;        // ���� ������ �ִ�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;            // ���� ���� ��Ÿ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ComponentName;        // ����ƽ �޽� ������Ʈ �̸�

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString ThorwingBPPath;        // ���� ��ô�� ��ȯ�� BP ���

	/// �� Ű�� ��ȯ�Ѵ�.
	int GetKey(){ return Num; };
};


// ��ų ����
USTRUCT( Atomic, BlueprintType )
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Num;                                           // �ĺ���

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;                                    // ��Ÿ��

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CostMP;                                      // �Ҹ� �� MP

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
	FString AnimPath;                                  // ��Ÿ�� ���

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMin;                            // ��ų �ߵ� ���� �ּ� (���� ����)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMax;                            // ��ų �ߵ� ���� �ִ� (���� ����)

	/// �� Ű�� ��ȯ�Ѵ�.
	int GetKey(){ return Num; };
};

// �÷��̾� �⺻ ��ų ����
USTRUCT( Atomic, BlueprintType )
struct FPlayerDefaultSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EInputKeyType InputKey;                          // �Է� Ű

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int SkillNum;                                    // �Է� Ű�� �ش��ϴ� ��ų �ѹ�

	/// �� Ű�� ��ȯ�Ѵ�.
	EInputKeyType GetKey(){ return InputKey; };
};

// �÷��̾� ���� ��ų ����
USTRUCT( Atomic, BlueprintType )
struct FPlayerWeaponSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int WeaponNum;                           // ���� �ĺ���

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int L_SkillNum;                          // ����� 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int R_SkillNum;                          // ������ 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int ThrowSkillNum;                       // ���� ��ô

	/// �� Ű�� ��ȯ�Ѵ�.
	int GetKey(){ return WeaponNum; };
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
	constexpr float HOLD_TIME_INCREASE_VALUE  = 0.1f;      // ������ �ð� ���� ��
	constexpr float MP_RECOVERY_VALUE         = 20.f;      // MP ȸ�� ��� 

	constexpr float DEFAULT_TARGET_ARM_LENGTH = 280.f;     // �⺻ ī�޶� �� ����
	constexpr float FLY_TARGET_ARM_LENGTH     = 400.f;     // ���� ī�޶� �� ����

	constexpr float DEFAULT_GRAVITY_SCALE     = 2.0f;      // �⺻ �߷�
	constexpr float FLY_GRAVITY_SCALE         = 0.3f;      // ���� �߷�

	constexpr float LOCKON_RANGE              = 800.f;      // ���� ����
	constexpr float LOCKON_CAMERA_ROTAION_LAG_SPEED = 5.f;  // ���� ī�޶� ȸ�� ������ �ӵ�
	constexpr float LOCKON_CAMERA_FIX_PITCH   = -15.f;      // ���� ī�޶� pitch ���� ��


	constexpr float MAX_MASS                  = 2.f;       // �ִ� ����
	constexpr float MAX_INTENSITY             = 2.f;       // �ִ� ����

	constexpr float MIDDLE_RATE               = 1.2f;      // �߰� ����/���� ����
	constexpr float HARD_RATE                 = 1.5f;      // �� ����/���� ����
};