// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	ACharacter* OwningCharacter;        // 부모 캐릭터 클래스

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EAnimState AnimState;       	    // 애니메이션 상태

	float      MoveSpeed;               // 이동속도
	bool       IsAttackMove;            // 공격중 이동 여부
	bool       IsEnabledAttackColl;     // 공격 콜리전 활성화 여부

public:	
	UGameObject();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 공격중 이동 여부를 반환한다.
	bool GetIsAttackMove() { return IsAttackMove; };

	// 공격중 이동 여부를 셋팅한다.
	void SetIsAttackMove( bool InIsAttackMove ){ IsAttackMove = InIsAttackMove; };

	// 공격 콜리전 활성화 여부를 반환한다.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// 공격 콜리전 활성화 여부를 셋팅한다.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	// 공격 콜리전 사이즈를 셋팅한다.
	void SetAttackCollSize( const FVector& InSize );

	// 공격 콜리전 위치를 셋팅한다.
	void SetAttackCollPos( const FVector& InPos );

	// 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// 애니메이션 상태를 교체한다.
	void _AnimStateChange();

	// 이동 관련 로직을 수행한다.
	void _Move();

	// 공격 콜리전을 초기화한다.
	void _ResetAttackColl();
};
