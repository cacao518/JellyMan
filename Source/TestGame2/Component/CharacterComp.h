// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "ObjectComp.h"
#include "CharacterComp.generated.h"


class UAnimMontage;


using CooltimeMap = TMap<int, float>; // (key:��ųID, value:������Ÿ��)


UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class TESTGAME2_API UCharacterComp final : public UObjectComp
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      HitAnim;                 // �ǰ� �ִϸ��̼�

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage*      LandAnim;                // ���� �ִϸ��̼�

private:
	ACharacter*        OwningCharacter;         // �θ� ĳ���� Ŭ����
	EAnimState         AnimState;       	    // �ִϸ��̼� ����
	FVector            MovePos;                 // �̵��� ��ġ
	CooltimeMap        CoolingSkills;           // ��Ÿ�� ���� �ִ� ��ų ����		
	const FSkillInfo*  CurSkillInfo;            // ���� ��ų ����     

	bool               IsForceMove;             // ���� �̵� ����(��ų �̵�, �˹�)
	bool               IsEnableDerivedKey;      // �߰� Ű �Է� �������� ����
	bool               LandOnce;                // ���� üũ ����

	float              HoldTime;                // ������ �ð� ����
	float              FallWaterTime;           // ���� �����ִ� �ð� ����
	float              MontagePlayTime;         // ��Ÿ�� ����ð� ����
	float              DeathTime;               // ��� �ð� ����

public:
	// ������ �Լ�
	UCharacterComp();

	// �Ҹ��� �Լ�
	~UCharacterComp();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ������ �ʱ�ȭ�Ѵ�.
	void ResetInfo( bool InForceReset = false );

	// ��Ÿ�ָ� �÷����Ѵ�.
	void MontagePlay( const FString& InMontagePath, float InScale = 1.f );

	// ��Ÿ�ָ� �÷����Ѵ�.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// ��ų�� �÷����Ѵ�.
	bool SkillPlay( int InSkillNum );

	// ���� ���� ó���� �Ѵ�.
	virtual void OnAttackSuccess() override;

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// �̵��ӵ��� �����Ѵ�.
	virtual void SetMoveSpeed( float InMoveSpeed ) override;

	// �������� �����Ѵ�.
	virtual void SetJumpPower( float InJumpPower ) override;

	// ���� �̵� ���θ� �����Ѵ�.
	void SetIsForceMove( bool InIsForceMove ){ IsForceMove = InIsForceMove; };

	// �߰� Ű �Է� �������� ���θ� �����Ѵ�.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// �̵��� ��ġ�� �����Ѵ�.
	void SetMovePos( float InMovePower, bool InIsKnockBack = false );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
	FString GetCurMontageName();

	// ���� �̵� ���θ� ��ȯ�Ѵ�.
	bool GetIsForceMove() { return IsForceMove; };

	// �߰� Ű �Է� �������� ���θ� ��ȯ�Ѵ�.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// �ִϸ��̼� ���¸� ��ȯ�Ѵ�.
	const EAnimState& GetAnimState() { return AnimState; };

	// ���� �ߵ� �� ��ų ������ ��ȯ�Ѵ�.
	const FSkillInfo* GetCurSkillInfo() { return CurSkillInfo; };

	// �ش� ��ų�� ��Ÿ���� ���� �ִ��� ���θ� ��ȯ�Ѵ�.
	bool IsCoolingSkill( int InSkillNum );

	// ��Ÿ�� ��� �ʱ�ð����� ���θ� ��ȯ�Ѵ�.
	bool IsMontageInitialTime();

	// ������ ���� ���θ� ��ȯ�Ѵ�. 
	bool IsHold();

protected:
	// ��� ���� ������ �����Ѵ�.
	virtual void _ProcessDie() override;

	// �ǰ� ó���� �Ѵ�.
	virtual void _ProcessHit( AActor* InOtherActor ) override;

private:
	// �ʱ�ȭ �Ѵ�.
	void _Init();

	// �ִϸ��̼� ���¸� ��ü�Ѵ�.
	void _AnimStateChange();

	// �̵� ���� ������ �����Ѵ�.
	void _ProcessMove();

	// ��ų ��Ÿ���� ����Ѵ�.
	void _RegisterCoolTime( const FSkillInfo& InSkillInfo );

	// ü�¹ٸ� ������Ʈ �Ѵ�.
	void _UpdateHpBar();

	// ��ų ��Ÿ���� ������.
	void _CoolingSkills( float InDeltaTime );

	// ���� ������ ó���� �Ѵ�.
	void _FallingWater( float InDeltaTime );

	// ���� ������ �����Ѵ�.
	void _ProcessLand();

	// ������ ������ �����Ѵ�.
	void _ProcessHold( float InDeltaTime );

	// Mp ������ �����Ѵ�.
	void _ProcessMp( float InDeltaTime );

	// �����ð� ���� ���� ó���� �Ѵ�.
	void _ProcessAccTime( float InDeltaTime );
};
