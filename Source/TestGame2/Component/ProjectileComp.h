// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectComp.h"
#include "ProjectileComp.generated.h"


UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class TESTGAME2_API UProjectileComp final : public UObjectComp
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FCollisionInfo     AttackCollInfo;          // 공격 콜리전 정보

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	float LifeTime;                             // 지속 시간

	FVector Direction;                          // 투사체 방향

private:
	float LifeTimeAcc;

public:
	// 생성자 함수
	UProjectileComp();

	// 소멸자 함수
	~UProjectileComp();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// 공격 성공 처리를 한다.
	virtual void OnAttackSuccess() override;

private:
	// 초기화 한다.
	void _Init();

	// 이동 관련 로직을 수행한다.
	void _ProcessMove();

	// 시간 관련 로직을 수행한다.
	void _ProcessTime( float InDeltaTime );
};
