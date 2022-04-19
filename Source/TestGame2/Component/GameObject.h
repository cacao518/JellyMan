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
	ACharacter*     OwningCharacter;         // �θ� ĳ���� Ŭ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EAnimState      AnimState;       	     // �ִϸ��̼� ����

	FCollisionInfo  AttackCollInfo;          // ���� �ݸ��� ����
	FVector         MovePos;                 // �̵��� ��ġ

	float           Hp;                      // ü��
	float           Hpm;                     // �ִ� ü��
	float           MoveSpeed;               // �̵��ӵ�
	float           AttackSpeed;             // ���ݼӵ�
	float			JumpPower;               // ������
	
	bool            IsDie;                   // ��� ����
	bool            IsAttackMove;            // ������ �̵� ����
	bool            IsEnabledAttackColl;     // ���� �ݸ��� Ȱ��ȭ ����
	bool            IsEnableDerivedKey;      // �߰� Ű �Է� �������� ����

public:	
	UGameObject();

	// Begin �Լ�
	virtual void BeginPlay() override;
	
	// Tick �Լ�
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ �ʱ�ȭ�Ѵ�.
	void ResetInfo( bool InForceReset = false );

	// ��Ÿ�ָ� �÷����Ѵ�.
	void MontagePlay( UAnimMontage* InMontage, float InScale = 1.f );

	// ī�޶� ����ũ�� �����Ѵ�.
	void CameraShake( float InScale = 1.f, bool InShakeByWeight = false );

	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////

	// ���� �ݸ��� ������ �����Ѵ�.
	void SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo );

	// HP, HPM�� �����Ѵ�.
	void SetHp( float InHp ){ Hp = InHp; Hpm = InHp; };
	
	// �̵��ӵ��� �����Ѵ�.
	void SetMoveSpeed( float InMoveSpeed );

	// �������� �����Ѵ�.
	void SetJumpPower( float InJumpPower );

	// ���ݼӵ��� �����Ѵ�.
	void SetAttackSpeed( float InAttackSpeed ){ AttackSpeed = InAttackSpeed; };

	// ������ �̵� ���θ� �����Ѵ�.
	void SetIsAttackMove( bool InIsAttackMove ){ IsAttackMove = InIsAttackMove; };

	// �߰� Ű �Է� �������� ���θ� �����Ѵ�.
	void SetIsEnableDerivedKey( bool InIsEnableDerivedKey ) { IsEnableDerivedKey = InIsEnableDerivedKey; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetIsEnabledAttackColl( bool InIsEnabledAttackColl );

	//// �̵��� ��ġ�� �����Ѵ�.
	void SetMovePos( float InMovePower );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////
	
	// ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
	FString GetCurMontageName();

	// ���� �ݸ��� ������ ��ȯ�Ѵ�.
	const FCollisionInfo& GetAttackCollInfo() { return AttackCollInfo; };
	
	// ���� HP�� ��ȯ�Ѵ�. 
	float GetHp() { return Hp; };

	// ���� �ݸ��� Ȱ��ȭ ���θ� ��ȯ�Ѵ�.
	bool GetIsEnabledAttackColl() { return IsEnabledAttackColl; };

	// ������ �̵� ���θ� ��ȯ�Ѵ�.
	bool GetIsAttackMove() { return IsAttackMove; };

	// �߰� Ű �Է� �������� ���θ� ��ȯ�Ѵ�.
	bool GetIsEnableDerivedKey() { return IsEnableDerivedKey; };

	// �ִϸ��̼� ���¸� ��ȯ�Ѵ�.
	EAnimState GetAnimState() { return AnimState; };

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
};
