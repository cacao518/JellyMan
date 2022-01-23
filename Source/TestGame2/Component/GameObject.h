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
	ACharacter* OwningCharacter;        // �θ� ĳ���� Ŭ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EAnimState AnimState;       	    // �ִϸ��̼� ����

	float      MoveSpeed;               // �̵��ӵ�
	bool       IsAttackMove;            // ������ �̵� ����
	bool       IsEnabledAttackColl;     // ���� �ݸ��� Ȱ��ȭ ����

public:	
	UGameObject();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ �̵� ���θ� ��ȯ�Ѵ�.
	bool GetIsAttackMove() { return IsAttackMove; };

	// ������ �̵� ���θ� �����Ѵ�.
	void SetIsAttackMove( bool InIsAttackMove ){ IsAttackMove = InIsAttackMove; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� ��ȯ�Ѵ�.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	// ���� �ݸ��� ����� �����Ѵ�.
	void SetAttackCollSize( const FVector& InSize );

	// ���� �ݸ��� ��ġ�� �����Ѵ�.
	void SetAttackCollPos( const FVector& InPos );

	// �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// �ִϸ��̼� ���¸� ��ü�Ѵ�.
	void _AnimStateChange();

	// �̵� ���� ������ �����Ѵ�.
	void _Move();

	// ���� �ݸ����� �ʱ�ȭ�Ѵ�.
	void _ResetAttackColl();
};
