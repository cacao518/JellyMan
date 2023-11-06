#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "SDB.generated.h"


using namespace std;


// 오브젝트 종류
UENUM( BlueprintType )
enum class EObjectType : uint8
{
	PC				  UMETA( DisplayName = "PC" ),
	NPC				  UMETA( DisplayName = "NPC" ),
	PROJECTILE        UMETA( DisplayName = "PROJECTILE" ),
	STATIC_OBJECT     UMETA( DisplayName = "STATIC_OBJECT" ),
	MAX,
};

// 애니메이션 상태
UENUM( BlueprintType )
enum class EAnimState : uint8
{
	IDLE_RUN            UMETA( DisplayName = "Idle_Run" ),
	JUMP                UMETA( DisplayName = "Jump" ),
	DIE                 UMETA( DisplayName = "Die" ),
	COMMON_ACTION       UMETA( DisplayName = "Common_Action" ),

	MAX,             
};

// 애니메이션 보조 상태
UENUM( BlueprintType )
enum class EAnimSubState : uint8
{
	DEFAULT             UMETA( DisplayName = "Default" ),
	MOVABLE             UMETA( DisplayName = "Movable" ),
	UPPER_LOWER_BLEND   UMETA( DisplayName = "Upper_Lower_Blend" ),

	MAX,
};

// 물질 상태
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

// 무기 상태
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	DEFAULT          UMETA( DisplayName = "Default" ),
	SWORD            UMETA( DisplayName = "Sword" ),
	AXE              UMETA( DisplayName = "Axe" ),
	SPEAR            UMETA( DisplayName = "Spear" ),

	MAX,
};

// 입력 키 종류
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

// 스킬 플레이 재생속도 종류
UENUM( BlueprintType )
enum class ESkillPlaySpeedType : uint8
{
	DEFAULT              UMETA( DisplayName = "Default" ),
	ATTACK_SPEED         UMETA( DisplayName = "AttackSpeed" ),
	MOVE_SPEED           UMETA( DisplayName = "MoveSpeed" ),

	MAX,
};

// 팀 종류
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
	float Mp; 	// 스태미너

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Mpm; 	// 스태미너

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MoveSpeed;	// 이동속도 ( 0 ~ 2 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackSpeed;	// 공격속도 ( 0 ~ 2 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackPower;	// 공격력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float DefensePower;	// 방어력

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float JumpPower;	// 점프력 ( 0 ~ 2 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Strength;     // 강인함 ( 0 ~ 100 ) ( 경직시간, 넉백거리 관련 ) 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Weight;   	// 무게 ( 0 ~ 2 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	bool IsTyrant;      // 폭군 여부 (팀 상관 없이 타격)
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// DataInfo
/////////////////////////////////////////////////////////////////////////////////////////////////////

// 물질 정보
USTRUCT( Atomic, BlueprintType )
struct FMaterialInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EMaterialState State;                            // 물질 상태

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Mass;                                      // 질량 ( 0 ~ 2 ) (공격속도,이동속도,점프력 비율)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float Intensity;                                 // 강도 ( 0 ~ 2 ) (공격력,방어력,강인함 비율 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float MatEnergyMax;                              // 최대 물질 에너지

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName CollisonName;                              // 물질 변경 시 콜리전이름

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray < FString > MaterialAssetPaths;         // 머터리얼 애셋경로 ( 해당 하는 머터리얼은 State가 될 수 있다 )

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString FootStepParticleName;                // 재질 밟을 때 스폰할 파티클 이름

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString ChangeParticleName;                  // 해당 재질로 변경되었을 때 스폰할 파티클 이름

	/// 맵 키를 반환한다.
	EMaterialState GetKey(){ return State; };
};

// 무기 정보
USTRUCT( Atomic, BlueprintType )
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Num;                    // 식별자

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EWeaponState State;         // 무기 종류

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int   DurabilityMax;        // 무기 내구도 최대

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;            // 무기 생성 쿨타임

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ComponentName;        // 스테틱 메쉬 컴포넌트 이름

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString ThorwingBPPath;        // 무기 투척시 소환할 BP 경로

	/// 맵 키를 반환한다.
	int GetKey(){ return Num; };
};


// 스킬 정보
USTRUCT( Atomic, BlueprintType )
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int Num;                                           // 식별자

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CoolTime;                                    // 쿨타임

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float CostMP;                                      // 소모 값 MP

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	ESkillPlaySpeedType PlaySpeedType;                 // 스킬 재생속도 타입

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EAnimSubState AnimSubState;                        // 애니메이션 보조 상태

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<EAnimState> PlayEnableState;                // 스킬 사용 가능한 상태

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<int> PlayEnableSkillNum;                    // 스킬 사용 가능한 스킬아이디

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int DerivedSkillNum;                               // 파생 스킬 식별자 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	bool LockOnLookAt;                                 // 락온 상태에서 스킬 시전 시 바라볼 것인지 여부 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FString AnimPath;                                  // 몽타주 경로

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMin;                            // 스킬 발동 범위 최소 (몬스터 전용)

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float ActivateRangeMax;                            // 스킬 발동 범위 최대 (몬스터 전용)

	/// 맵 키를 반환한다.
	int GetKey(){ return Num; };
};

// 플레이어 기본 스킬 정보
USTRUCT( Atomic, BlueprintType )
struct FPlayerDefaultSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EInputKeyType InputKey;                          // 입력 키

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int SkillNum;                                    // 입력 키에 해당하는 스킬 넘버

	/// 맵 키를 반환한다.
	EInputKeyType GetKey(){ return InputKey; };
};

// 플레이어 무기 스킬 정보
USTRUCT( Atomic, BlueprintType )
struct FPlayerWeaponSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int WeaponNum;                           // 무기 식별자

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int L_SkillNum;                          // 약공격 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int R_SkillNum;                          // 강공격 

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int ThrowSkillNum;                       // 무기 투척

	/// 맵 키를 반환한다.
	int GetKey(){ return WeaponNum; };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// Const
/////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Const
{ 
    // Common
	constexpr float ANIM_LERP_MULITPLIER      = 5.f;       // 애니메이션 중 이동 속도 보간 값
	constexpr float DEFAULT_MOVE_SPEED        = 450.f;     // 기본 이동속도
	constexpr float DEFAULT_JUMP_POWER        = 750.f;     // 기본 점프력
	constexpr float DEFAULT_ROTATION_RATE     = 1000.f;    // 기본 캐릭터 방향 전환 비율
	constexpr float FLY_ROTATION_RATE         = 100.f;     // 날기 캐릭터 방향 전환 비율
	constexpr float WATER_FALL_ANIM_TIME      = 0.01f;     // 물에 빠지는 애니메이션 속도
	constexpr float READY_SKILL_ROTATION_RATE = 100000.f;  // 발동 대기 스킬 보유 캐릭터 방향 전환 비율
	constexpr float READY_SKILL_DURATION      = 0.4f;      // 발동 대기중인 스킬 저장 지속시간
	constexpr float MONTAGE_INITIAL_TIME      = 0.05f;     // 몽타주 발동 후 초기시간 ( 방향전환가능 및 LookAt 시간)
	constexpr float DEAD_ACTOR_DESTROY_TIME   = 3.f;       // 사망 후 해당 초 이후 액터 삭제
	constexpr float HOLD_TIME_INCREASE_VALUE  = 0.1f;      // 역경직 시간 증가 값
	constexpr float MP_RECOVERY_VALUE         = 20.f;      // MP 회복 상수 

	constexpr float DEFAULT_TARGET_ARM_LENGTH = 280.f;     // 기본 카메라 암 길이
	constexpr float FLY_TARGET_ARM_LENGTH     = 400.f;     // 날기 카메라 암 길이

	constexpr float DEFAULT_GRAVITY_SCALE     = 2.0f;      // 기본 중력
	constexpr float FLY_GRAVITY_SCALE         = 0.3f;      // 날기 중력

	constexpr float LOCKON_RANGE              = 800.f;      // 락온 범위
	constexpr float LOCKON_CAMERA_ROTAION_LAG_SPEED = 5.f;  // 락온 카메라 회전 딜레이 속도
	constexpr float LOCKON_CAMERA_FIX_PITCH   = -15.f;      // 락온 카메라 pitch 고정 값


	constexpr float MAX_MASS                  = 2.f;       // 최대 질량
	constexpr float MAX_INTENSITY             = 2.f;       // 최대 강도

	constexpr float MIDDLE_RATE               = 1.2f;      // 중간 강도/질량 기준
	constexpr float HARD_RATE                 = 1.5f;      // 고 강도/질량 기준
};