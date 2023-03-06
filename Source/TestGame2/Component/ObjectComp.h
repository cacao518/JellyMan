// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UObjectComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	int                Id;                      // 오브젝트 아이디 값

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FStatusInfo        Stat;                    // 능력치

protected:
	AActor*            OwningActor;             // 부모 액터 클래스
	ETeamType          TeamType;                // 팀 타입
	FCollisionInfo     AttackCollInfo;          // 공격 콜리전 정보
	FStatusInfo        InitStat;                // 초기기본 능력치

	bool               IsDie;                   // 사망 여부
	bool               IsFallWater;             // 물에 빠졌는지 여부

public:	
	// 생성자 함수
	UObjectComp();

	// 소멸자 함수
	~UObjectComp();

	// Begin 함수
	virtual void BeginPlay() override;
	
	// Tick 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 대상을 바라본다.
	void LookAt( ACharacter* InTarget );

	// 공격 성공 처리를 한다.
	virtual void OnAttackSuccess() {};

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// 오브젝트 아이디를 셋팅한다.
	void SetId( int InId ){ Id = InId; };

	// 팀 타입을 설정한다.
	void SetTeamType( ETeamType InTeamType ){ TeamType = InTeamType; };

	// 공격 콜리전 정보를 셋팅한다.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM을 설정한다.
	void SetHp( float InHp ){ Stat.Hp = InHp; Stat.Hpm = InHp; };
	
	// 이동속도를 설정한다.
	virtual void SetMoveSpeed( float InMoveSpeed );

	// 점프력을 설정한다.
	virtual void SetJumpPower( float InJumpPower );

	// 공격속도를 설정한다.
	void SetAttackSpeed( float InAttackSpeed ){ Stat.AttackSpeed = InAttackSpeed; };

	// 공격력을 설정한다.
	void SetAttackPower( float InAttackPower ) { Stat.AttackPower = InAttackPower; };

	// 방어력을 설정한다.
	void SetDefensePower( float InDefensePower ){ Stat.DefensePower = InDefensePower; };

	// 강인함을 설정한다.
	void SetStrength( float InStrength ){ Stat.Strength = InStrength; };

	// 무게를 설정한다.
	void SetWeight( float InWeight ){ Stat.Weight = InWeight; };

	// 공격 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledAttackColl( bool InIsEnabled );

	// 히트 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledHitColl( bool InIsEnabled );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

    // 오브젝트 타입을 알아낸다.
	EObjectType GetObjectType();

	// 팀 타입을 반환한다.
	ETeamType GetTeamType() { return TeamType; };

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

	// 사망 여부를 반환한다.
	bool GetIsDie(){ return IsDie; };

	// 고 중량 인지 여부를 반환한다.
	bool IsHeavyWeight();

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////
	
	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

protected:
	// 카메라 쉐이크 처리를 한다.
	void _ProcessCameraShake( AActor* InOtherActor );

	// 사망 관련 로직을 수행한다.
	virtual void _ProcessDie();

	// 피격 처리를 한다.
	virtual void _ProcessHit( AActor* InOtherActor );

private:
	// 초기화 한다.
	void _Init();

	// 물 히트 처리를 한다.
	void _ProcessWaterHit( AActor* InOtherActor );
};
