// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "ObjectComp.h"
#include "CharacterComp.generated.h"


class UAnimMontage;


using CooltimeMap = TMap<int, float>; // (key:스킬ID, value:남은쿨타임)


UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class TESTGAME2_API UCharacterComp final : public UObjectComp
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      HitAnim;                 // 피격 애니메이션

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      LandAnim;                // 착지 애니메이션

private:
	ACharacter*        OwningCharacter;         // 부모 캐릭터 클래스
	EAnimState         AnimState;       	    // 애니메이션 상태
	FVector            MovePos;                 // 이동할 위치
	CooltimeMap        CoolingSkills;           // 쿨타임 돌고 있는 스킬 정보		
	const FSkillInfo*  CurSkillInfo;            // 현재 스킬 정보     

	bool               IsForceMove;             // 강제 이동 여부(스킬 이동, 넉백)
	bool               IsEnableDerivedKey;      // 추가 키 입력 가능한지 여부
	bool               LandOnce;                // 착지 체크 변수

	float              HoldTime;                // 역경직 시간 누적
	float              FallWaterTime;           // 물에 빠져있는 시간 누적
	float              MontagePlayTime;         // 몽타주 재생시간 누적
	float              DeathTime;               // 사망 시간 누적

public:
	// 생성자 함수
	UCharacterComp();

	// 소멸자 함수
	~UCharacterComp();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// 정보를 초기화한다.
	void ResetInfo( bool InForceReset = false );

	// 몽타주를 플레이한다.
	void MontagePlay( const FString& InMontagePath, float InScale = 1.f );

	// 몽타주를 플레이한다.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// 스킬을 플레이한다.
	bool SkillPlay( int InSkillNum );

	// 공격 성공 처리를 한다.
	virtual void OnAttackSuccess() override;

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// 이동속도를 설정한다.
	virtual void SetMoveSpeed( float InMoveSpeed ) override;

	// 점프력을 설정한다.
	virtual void SetJumpPower( float InJumpPower ) override;

	// 강제 이동 여부를 셋팅한다.
	void SetIsForceMove( bool InIsForceMove ){ IsForceMove = InIsForceMove; };

	// 추가 키 입력 가능한지 여부를 셋팅한다.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// 이동할 위치를 셋팅한다.
	void SetMovePos( float InMovePower, bool InIsKnockBack = false );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// 현재 몽타주 이름을 반환한다.
	FString GetCurMontageName();

	// 강제 이동 여부를 반환한다.
	bool GetIsForceMove() { return IsForceMove; };

	// 추가 키 입력 가능한지 여부를 반환한다.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// 애니메이션 상태를 반환한다.
	const EAnimState& GetAnimState() { return AnimState; };

	// 현재 발동 중 스킬 정보를 반환한다.
	const FSkillInfo* GetCurSkillInfo() { return CurSkillInfo; };

	// 해당 스킬이 쿨타임이 돌고 있는지 여부를 반환한다.
	bool IsCoolingSkill( int InSkillNum );

	// 몽타주 재생 초기시간인지 여부를 반환한다.
	bool IsMontageInitialTime();

	// 역경직 상태 여부를 반환한다. 
	bool IsHold();

protected:
	// 사망 관련 로직을 수행한다.
	virtual void _ProcessDie() override;

	// 피격 처리를 한다.
	virtual void _ProcessHit( AActor* InOtherActor ) override;

private:
	// 초기화 한다.
	void _Init();

	// 애니메이션 상태를 교체한다.
	void _AnimStateChange();

	// 이동 관련 로직을 수행한다.
	void _ProcessMove();

	// 스킬 쿨타임을 등록한다.
	void _RegisterCoolTime( const FSkillInfo& InSkillInfo );

	// 체력바를 업데이트 한다.
	void _UpdateHpBar();

	// 스킬 쿨타임을 돌린다.
	void _CoolingSkills( float InDeltaTime );

	// 물에 빠지는 처리를 한다.
	void _FallingWater( float InDeltaTime );

	// 착지 로직을 실행한다.
	void _ProcessLand();

	// 역경직 로직을 수행한다.
	void _ProcessHold( float InDeltaTime );

	// Mp 로직을 수행한다.
	void _ProcessMp( float InDeltaTime );

	// 누적시간 관련 로직 처리를 한다.
	void _ProcessAccTime( float InDeltaTime );
};
