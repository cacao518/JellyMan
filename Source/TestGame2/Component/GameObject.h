// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


class UAnimMontage;
class AActorSpawner;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	FStatusInfo        Stat;                    // �ɷ�ġ

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	TArray<FSkillInfo> SkillInfos;              // �ش� BP ��� ��ų ����

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      HitAnim;                 // �ǰ� �ִϸ��̼�

private:
	int                Id;                      // ������Ʈ ���̵� ��
	ACharacter*        OwningCharacter;         // �θ� ĳ���� Ŭ����
	EAnimState         AnimState;       	    // �ִϸ��̼� ����
	FCollisionInfo     AttackCollInfo;          // ���� �ݸ��� ����
	FVector            MovePos;                 // �̵��� ��ġ
				       
	bool               IsDie;                   // ��� ����
	bool               IsForceMove;             // ���� �̵� ����(��ų �̵�, �˹�)
	bool               IsEnabledAttackColl;     // ���� �ݸ��� Ȱ��ȭ ����
	bool               IsEnableDerivedKey;      // �߰� Ű �Է� �������� ����

	TMap<int, float>   CoolingSkills;           // ��Ÿ�� ���� �ִ� ��ų ���� (key:��ųID, value:������Ÿ��)

	// Player ����
	FVector            MoveDirectionInAction;   // �⺻ ���� ĵ�� �� �������� �̵��� ����
	bool               AttackCanceling;         // �⺻ ������ ĵ�����ΰ�

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
	bool SkillPlay( int InSkillNum, float InScale = 1.f );

	// ����� �ٶ󺻴�.
	void LookAt( ACharacter* InTarget );

	// ī�޶� ����ũ�� �����Ѵ�.
	void CameraShake( float InScale = 1.f, bool InShakeByWeight = false );

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

	// ���� �̵� ���θ� �����Ѵ�.
	void SetIsForceMove( bool InIsForceMove ){ IsForceMove = InIsForceMove; };

	// �߰� Ű �Է� �������� ���θ� �����Ѵ�.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	// �̵��� ��ġ�� �����Ѵ�.
	void SetMovePos( float InMovePower, bool InIsKnockBack = false);

	// �⺻ ���� ĵ�� �� �������� �̵��� ���� ���� ���Ѵ�.
	void AddMoveDirectionInAction( FVector InDirection, float InValue );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////
	
	// ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
	FString GetCurMontageName();

	// ���� �ݸ��� ������ ��ȯ�Ѵ�.
	const FCollisionInfo& GetAttackCollInfo() { return AttackCollInfo; };

	// ������Ʈ ���̵� ��ȯ�Ѵ�.
	int GetId(){ return Id; };

	// ���� �ɷ�ġ�� ��ȯ�Ѵ�.
	const FStatusInfo& GetStat() { return Stat; };
	
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
};
