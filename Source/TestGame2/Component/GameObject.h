// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


class UAnimMontage;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	ACharacter*     OwningCharacter;         // 부모 캐릭터 클래스

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EAnimState      AnimState;       	     // 애니메이션 상태

	FCollisionInfo  AttackCollInfo;          // 공격 콜리전 정보
	FVector         MovePos;                 // 이동할 위치

	float           Hp;                      // 체력
	float           Hpm;                     // 최대 체력
	float           MoveSpeed;               // 이동속도
	float           AttackSpeed;             // 공격속도
	float			JumpPower;               // 점프력
	
	bool            IsDie;                   // 사망 여부
	bool            IsAttackMove;            // 공격중 이동 여부
	bool            IsEnabledAttackColl;     // 공격 콜리전 활성화 여부
	bool            IsEnableDerivedKey;      // 추가 키 입력 가능한지 여부

public:	
	UGameObject();

	// Begin 함수
	virtual void BeginPlay() override;
	
	// Tick 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 정보를 초기화한다.
	void ResetInfo( bool InForceReset = false );

	// 몽타주를 플레이한다.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// 카메라 쉐이크를 실행한다.
	void CameraShake( float InScale = 1.f, bool InShakeByWeight = false );

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// 공격 콜리전 정보를 셋팅한다.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM을 설정한다.
	void SetHp( float InHp ){ Hp = InHp; Hpm = InHp; };
	
	// 이동속도를 설정한다.
	void SetMoveSpeed( float InMoveSpeed );

	// 점프력을 설정한다.
	void SetJumpPower( float InJumpPower );

	// 공격속도를 설정한다.
	void SetAttackSpeed( float InAttackSpeed ){ AttackSpeed = InAttackSpeed; };

	// 공격중 이동 여부를 셋팅한다.
	void SetIsAttackMove( bool InIsAttackMove ){ IsAttackMove = InIsAttackMove; };

	// 추가 키 입력 가능한지 여부를 셋팅한다.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// 공격 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	//// 이동할 위치를 셋팅한다.
	void SetMovePos( float InMovePower );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////
	
	// 현재 몽타주 이름을 반환한다.
	FString GetCurMontageName();

	// 공격 콜리전 정보를 반환한다.
	const FCollisionInfo& GetAttackCollInfo() { return AttackCollInfo; };
	
	// 현재 HP를 반환한다. 
	float GetHp() { return Hp; };

	// 공격 콜리전 활성화 여부를 반환한다.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// 공격중 이동 여부를 반환한다.
	bool GetIsAttackMove() { return IsAttackMove; };

	// 추가 키 입력 가능한지 여부를 반환한다.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// 애니메이션 상태를 반환한다.
	EAnimState GetAnimState() { return AnimState; };

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////
	
	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// 애니메이션 상태를 교체한다.
	void _AnimStateChange();

	// 이동 관련 로직을 수행한다.
	void _Move();

	// 해당 캐릭터가 사망했는지 체크한다.
	void _CheckDie();
};
