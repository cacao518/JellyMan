#pragma once

#include "CoreMinimal.h"
#include "SDB.generated.h"


using namespace std;


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
	MARBLE           UMETA( DisplayName = "Marble" ),

	MAX,
};

// 무기 상태
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	SWORD            UMETA( DisplayName = "Sword" ),

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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// DataInfo
/////////////////////////////////////////////////////////////////////////////////////////////////////

// 물질 정보
struct MaterialInfo
{
	float MoveSpeed;         // 이동속도 증가율
	float AttackSpeed;       // 공격속도 증가율
	float JumpPower;         // 점프력
	float Mass;              // 강도(공격력,방어력 증가율)
	float MatEnergyMax;      // 최대 물질 에너지
	FName CollisonName;      // 물질 변경 시 콜리전이름
	FString AssetPath;       // 애셋 경로
};

// 무기 정보
struct WeaponInfo
{
	int   DurabilityMax;        // 무기 내구도 최대
	float RequireJellyAmount;   // 무기를 생성하는데 필요한 젤리 양
	FName ComponentName;        // 스테틱 메쉬 컴포넌트 이름
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
	constexpr float READY_SKILL_ROTATION_RATE = 100000.f;  // 발동 대기 스킬 보유 캐릭터 방향 전환 비율
	constexpr float READY_SKILL_DURATION      = 0.4f;      // 발동 대기중인 스킬 저장 지속시간

	constexpr float DEFAULT_TARGET_ARM_LENGTH = 280.f;     // 기본 카메라 암 길이
	constexpr float FLY_TARGET_ARM_LENGTH     = 400.f;     // 날기 카메라 암 길이

	constexpr float DEFAULT_GRAVITY_SCALE     = 2.0f;      // 기본 중력
	constexpr float FLY_GRAVITY_SCALE         = 0.3f;      // 날기 중력

	constexpr float LOCKON_RANGE = 800.f;                  // 락온 범위
	constexpr float LOCKON_CAMERA_ROTAION_LAG_SPEED = 5.f; // 락온 카메라 회전 딜레이 속도
};