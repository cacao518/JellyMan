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
	int                Id;                      // ������Ʈ ���̵� ��

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FStatusInfo        Stat;                    // �ɷ�ġ

protected:
	AActor*            OwningActor;             // �θ� ���� Ŭ����
	ETeamType          TeamType;                // �� Ÿ��
	FCollisionInfo     AttackCollInfo;          // ���� �ݸ��� ����
	FStatusInfo        InitStat;                // �ʱ�⺻ �ɷ�ġ

	bool               IsDie;                   // ��� ����
	bool               IsFallWater;             // ���� �������� ����

public:	
	// ������ �Լ�
	UObjectComp();

	// �Ҹ��� �Լ�
	~UObjectComp();

	// Begin �Լ�
	virtual void BeginPlay() override;
	
	// Tick �Լ�
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ����� �ٶ󺻴�.
	void LookAt( ACharacter* InTarget );

	// ���� ���� ó���� �Ѵ�.
	virtual void OnAttackSuccess() {};

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// ������Ʈ ���̵� �����Ѵ�.
	void SetId( int InId ){ Id = InId; };

	// �� Ÿ���� �����Ѵ�.
	void SetTeamType( ETeamType InTeamType ){ TeamType = InTeamType; };

	// ���� �ݸ��� ������ �����Ѵ�.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM�� �����Ѵ�.
	void SetHp( float InHp ){ Stat.Hp = InHp; Stat.Hpm = InHp; };
	
	// �̵��ӵ��� �����Ѵ�.
	virtual void SetMoveSpeed( float InMoveSpeed );

	// �������� �����Ѵ�.
	virtual void SetJumpPower( float InJumpPower );

	// ���ݼӵ��� �����Ѵ�.
	void SetAttackSpeed( float InAttackSpeed ){ Stat.AttackSpeed = InAttackSpeed; };

	// ���ݷ��� �����Ѵ�.
	void SetAttackPower( float InAttackPower ) { Stat.AttackPower = InAttackPower; };

	// ������ �����Ѵ�.
	void SetDefensePower( float InDefensePower ){ Stat.DefensePower = InDefensePower; };

	// �������� �����Ѵ�.
	void SetStrength( float InStrength ){ Stat.Strength = InStrength; };

	// ���Ը� �����Ѵ�.
	void SetWeight( float InWeight ){ Stat.Weight = InWeight; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledAttackColl( bool InIsEnabled );

	// ��Ʈ �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledHitColl( bool InIsEnabled );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

    // ������Ʈ Ÿ���� �˾Ƴ���.
	EObjectType GetObjectType();

	// �� Ÿ���� ��ȯ�Ѵ�.
	ETeamType GetTeamType() { return TeamType; };

	// ���� �ݸ��� ������ ��ȯ�Ѵ�.
	const FCollisionInfo& GetAttackCollInfo() { return AttackCollInfo; };

	// ������Ʈ ���̵� ��ȯ�Ѵ�.
	int GetId(){ return Id; };

	// ���� �ɷ�ġ�� ��ȯ�Ѵ�.
	const FStatusInfo& GetStat() { return Stat; };

	// �ʱ� �⺻ �ɷ�ġ�� ��ȯ�Ѵ�.
	const FStatusInfo& GetInitStat() { return InitStat; };
	
	// ���� HP�� ��ȯ�Ѵ�. 
	float GetHp() { return Stat.Hp; };

	// ��� ���θ� ��ȯ�Ѵ�.
	bool GetIsDie(){ return IsDie; };

	// �� �߷� ���� ���θ� ��ȯ�Ѵ�.
	bool IsHeavyWeight();

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////
	
	// �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

protected:
	// ī�޶� ����ũ ó���� �Ѵ�.
	void _ProcessCameraShake( AActor* InOtherActor );

	// ��� ���� ������ �����Ѵ�.
	virtual void _ProcessDie();

	// �ǰ� ó���� �Ѵ�.
	virtual void _ProcessHit( AActor* InOtherActor );

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// �� ��Ʈ ó���� �Ѵ�.
	void _ProcessWaterHit( AActor* InOtherActor );
};
