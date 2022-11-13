// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


class UAnimMontage;
class AActorSpawner;


using CooltimeMap = TMap<int, float>; // (key:��ųID, value:������Ÿ��)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	int                Id;                      // ������Ʈ ���̵� ��

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FStatusInfo        Stat;                    // �ɷ�ġ

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	TArray<FSkillInfo> SkillInfos;              // �ش� BP ��� ��ų ����

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      HitAnim;                 // �ǰ� �ִϸ��̼�

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      LandAnim;                // ���� �ִϸ��̼�

private:
	ACharacter*        OwningCharacter;         // �θ� ĳ���� Ŭ����
	EAnimState         AnimState;       	    // �ִϸ��̼� ����
	FCollisionInfo     AttackCollInfo;          // ���� �ݸ��� ����
	FVector            MovePos;                 // �̵��� ��ġ
	FStatusInfo        InitStat;                // �ʱ�⺻ �ɷ�ġ
	CooltimeMap        CoolingSkills;           // ��Ÿ�� ���� �ִ� ��ų ����			       

	bool               IsDie;                   // ��� ����
	bool               IsFallWater;             // ���� �������� ����
	bool               IsForceMove;             // ���� �̵� ����(��ų �̵�, �˹�)
	bool               IsEnabledAttackColl;     // ���� �ݸ��� Ȱ��ȭ ����
	bool               IsEnableDerivedKey;      // �߰� Ű �Է� �������� ����
	bool               LandOnce;                // ���� üũ ����
	float              FallWaterTimeAmount;     // ���� �����ִ� �ð�

public:	

	// ������ �Լ�
	UGameObject();

	// �Ҹ��� �Լ�
	~UGameObject();

	// Begin �Լ�
	virtual void BeginPlay() override;
	
	// Tick �Լ�
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ �ʱ�ȭ�Ѵ�.
	void ResetInfo( bool InForceReset = false );

	// ��Ÿ�ָ� �÷����Ѵ�.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// ��ų�� �÷����Ѵ�.
	bool SkillPlay( int InSkillNum );

	// ����� �ٶ󺻴�.
	void LookAt( ACharacter* InTarget );

	// ���� ���� ó���� �Ѵ�.
	void OnAttackSuccess();

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// ������Ʈ ���̵� �����Ѵ�.
	void SetId( int InId ){ Id = InId; };

	// ���� �ݸ��� ������ �����Ѵ�.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM�� �����Ѵ�.
	void SetHp( float InHp ){ Stat.Hp = InHp; Stat.Hpm = InHp; };
	
	// �̵��ӵ��� �����Ѵ�.
	void SetMoveSpeed( float InMoveSpeed );

	// �������� �����Ѵ�.
	void SetJumpPower( float InJumpPower );

	// ���ݼӵ��� �����Ѵ�.
	void SetAttackSpeed( float InAttackSpeed ){ Stat.AttackSpeed = InAttackSpeed; };

	// ���ݷ��� �����Ѵ�.
	void SetAttackPower( float InAttackPower ) { Stat.AttackPower = InAttackPower; } ;

	// ������ �����Ѵ�.
	void SetDefensePower( float InDefensePower ){ Stat.DefensePower = InDefensePower; };

	// �������� �����Ѵ�.
	void SetStrength( float InStrength ){ Stat.Strength = InStrength; };

	// ���� �̵� ���θ� �����Ѵ�.
	void SetIsForceMove( bool InIsForceMove ){ IsForceMove = InIsForceMove; };

	// �߰� Ű �Է� �������� ���θ� �����Ѵ�.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	// �̵��� ��ġ�� �����Ѵ�.
	void SetMovePos( float InMovePower, bool InIsKnockBack = false);

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////
	
	// ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
	FString GetCurMontageName();

    // ������Ʈ Ÿ���� �˾Ƴ���.
	EObjectType GetObjectType();

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

	// ���� �ݸ��� Ȱ��ȭ ���θ� ��ȯ�Ѵ�.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// ���� �̵� ���θ� ��ȯ�Ѵ�.
	bool GetIsForceMove() { return IsForceMove; };

	// �߰� Ű �Է� �������� ���θ� ��ȯ�Ѵ�.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// �ִϸ��̼� ���¸� ��ȯ�Ѵ�.
	const EAnimState& GetAnimState() { return AnimState; };

	// ��ų ������ ��ȯ�Ѵ�.
	const TArray<FSkillInfo>& GetSkillInfos() { return SkillInfos; };

	// �ش� ��ų�� ��Ÿ���� ���� �ִ��� ���θ� ��ȯ�Ѵ�.
	bool IsCoolingSkill( int InSkillNum );

	///////////////////////////////////////////////////////////////////////
	// Delegate Function
	///////////////////////////////////////////////////////////////////////
	
	// �浹�� �����ҽÿ� ȣ��Ǵ� ��������Ʈ�� ����ϴ� �Լ�
	UFUNCTION()
	void HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// �ִϸ��̼� ���¸� ��ü�Ѵ�.
	void _AnimStateChange();

	// �̵� ���� ������ �����Ѵ�.
	void _Move();

	// �ش� ĳ���Ͱ� ����ߴ��� üũ�Ѵ�.
	void _CheckDie();

	// ��ų ��Ÿ���� ����Ѵ�.
	void _RegisterCoolTime( const FSkillInfo& InSkillInfo );

	// ��ų ��Ÿ���� ������.
	void _CoolingSkills( float InDeltaTime );

	// ���� ������ ó���� �Ѵ�.
	void _FallingWater( float InDeltaTime );

	// �ǰ� ó���� �Ѵ�.
	void _ProcessHit( AActor* InOtherActor );

	// ���彺������ �ǰ� ó���� �Ѵ�.
	void _ProcessLandscapeHit( AActor* InOtherActor );

	// ���� ������ �����Ѵ�.
	void _ProcessLand();
};
