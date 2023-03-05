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
	FCollisionInfo     AttackCollInfo;          // ���� �ݸ��� ����

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	float LifeTime;                             // ���� �ð�

	FVector Direction;                          // ����ü ����

private:
	float LifeTimeAcc;

public:
	// ������ �Լ�
	UProjectileComp();

	// �Ҹ��� �Լ�
	~UProjectileComp();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ���� ���� ó���� �Ѵ�.
	virtual void OnAttackSuccess() override;

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// �̵� ���� ������ �����Ѵ�.
	void _ProcessMove();

	// �ð� ���� ������ �����Ѵ�.
	void _ProcessTime( float InDeltaTime );
};
