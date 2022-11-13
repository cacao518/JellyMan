// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


class UAnimMontage;
class AActorSpawner;


using CooltimeMap = TMap<int, float>; // (key:스킬ID, value:남은쿨타임)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	int                Id;                      // 오브젝트 아이디 값

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FStatusInfo        Stat;                    // 능력치

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	TArray<FSkillInfo> SkillInfos;              // 해당 BP 모든 스킬 정보

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      HitAnim;                 // 피격 애니메이션

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      LandAnim;                // 착지 애니메이션

private:
	ACharacter*        OwningCharacter;         // 부모 캐릭터 클래스
	EAnimState         AnimState;       	    // 애니메이션 상태
	FCollisionInfo     AttackCollInfo;          // 공격 콜리전 정보
	FVector            MovePos;                 // 이동할 위치
	FStatusInfo        InitStat;                // 초기기본 능력치
	CooltimeMap        CoolingSkills;           // 쿨타임 돌고 있는 스킬 정보			       

	bool               IsDie;                   // 사망 여부
	bool               IsFallWater;             // 물에 빠졌는지 여부
	bool               IsForceMove;             // 강제 이동 여부(스킬 이동, 넉백)
	bool               IsEnabledAttackColl;     // 공격 콜리전 활성화 여부
	bool               IsEnableDerivedKey;      // 추가 키 입력 가능한지 여부
	bool               LandOnce;                // 착지 체크 변수
	float              FallWaterTimeAmount;     // 물에 빠져있는 시간

public:	

	// 생성자 함수
	UGameObject();

	// 소멸자 함수
	~UGameObject();

	// Begin 함수
	virtual void BeginPlay() override;
	
	// Tick 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 정보를 초기화한다.
	void ResetInfo( bool InForceReset = false );

	// 몽타주를 플레이한다.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// 스킬을 플레이한다.
	bool SkillPlay( int InSkillNum );

	// 대상을 바라본다.
	void LookAt( ACharacter* InTarget );

	// 공격 성공 처리를 한다.
	void OnAttackSuccess();

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// 오브젝트 아이디를 셋팅한다.
	void SetId( int InId ){ Id = InId; };

	// 공격 콜리전 정보를 셋팅한다.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM을 설정한다.
	void SetHp( float InHp ){ Stat.Hp = InHp; Stat.Hpm = InHp; };
	
	// 이동속도를 설정한다.
	void SetMoveSpeed( float InMoveSpeed );

	// 점프력을 설정한다.
	void SetJumpPower( float InJumpPower );

	// 공격속도를 설정한다.
	void SetAttackSpeed( float InAttackSpeed ){ Stat.AttackSpeed = InAttackSpeed; };

	// 공격력을 설정한다.
	void SetAttackPower( float InAttackPower ) { Stat.AttackPower = InAttackPower; } ;

	// 방어력을 설정한다.
	void SetDefensePower( float InDefensePower ){ Stat.DefensePower = InDefensePower; };

	// 강인함을 설정한다.
	void SetStrength( float InStrength ){ Stat.Strength = InStrength; };

	// 강제 이동 여부를 셋팅한다.
	void SetIsForceMove( bool InIsForceMove ){ IsForceMove = InIsForceMove; };

	// 추가 키 입력 가능한지 여부를 셋팅한다.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// 공격 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	// 이동할 위치를 셋팅한다.
	void SetMovePos( float InMovePower, bool InIsKnockBack = false);

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////
	
	// 현재 몽타주 이름을 반환한다.
	FString GetCurMontageName();

    // 오브젝트 타입을 알아낸다.
	EObjectType GetObjectType();

	// 공격 콜리전 정보를 반환한다.
	const FCollisionInfo& GetAttackCollInfo() { return AttackCollInfo; };

	// 오브젝트 아이디를 반환한다.
	int GetId(){ return Id; };

	// 현재 능력치를 반환한다.
	const FStatusInfo& GetStat() { return Stat; };

	// 초기 기본 능력치를 반환한다.
	const FStatusInfo& GetInitStat() { return InitStat; };
	
	// 현재 HP를 반환한다. 
	float GetHp() { return Stat.Hp; };

	// 공격 콜리전 활성화 여부를 반환한다.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// 강제 이동 여부를 반환한다.
	bool GetIsForceMove() { return IsForceMove; };

	// 추가 키 입력 가능한지 여부를 반환한다.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// 애니메이션 상태를 반환한다.
	const EAnimState& GetAnimState() { return AnimState; };

	// 스킬 정보를 반환한다.
	const TArray<FSkillInfo>& GetSkillInfos() { return SkillInfos; };

	// 해당 스킬이 쿨타임이 돌고 있는지 여부를 반환한다.
	bool IsCoolingSkill( int InSkillNum );

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////
	
	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// 초기화 한다.
	void _Init();

	// 애니메이션 상태를 교체한다.
	void _AnimStateChange();

	// 이동 관련 로직을 수행한다.
	void _Move();

	// 해당 캐릭터가 사망했는지 체크한다.
	void _CheckDie();

	// 스킬 쿨타임을 등록한다.
	void _RegisterCoolTime( const FSkillInfo& InSkillInfo );

	// 스킬 쿨타임을 돌린다.
	void _CoolingSkills( float InDeltaTime );

	// 물에 빠지는 처리를 한다.
	void _FallingWater( float InDeltaTime );

	// 피격 처리를 한다.
	void _ProcessHit( AActor* InOtherActor );

	// 랜드스케이프 피격 처리를 한다.
	void _ProcessLandscapeHit( AActor* InOtherActor );

	// 착지 로직을 실행한다.
	void _ProcessLand();
};
