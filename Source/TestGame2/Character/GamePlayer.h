// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayer.generated.h"

using namespace std;

class UAnimMontage;
class USkeletalMeshComponent;


UCLASS(config=Game)
class AGamePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UGameObject* GameObject;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UMaterialProperty* MatProperty;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UWeaponChange* WeaponChange;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* AttackColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* TileColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

private:
	float                 ReadySkillResetTime;  // 발동 대기중인 스킬 초기화 시간
	FVector               ReadySkillDirection;  // 발동 대기중인 마지막으로 입력한 스킬의 방향
	function<void()>      ReadySkillFunc;       // 발동 대기중인 마지막으로 입력한 스킬 함수

public:
	AGamePlayer();

	virtual void BeginPlay() override;

	virtual void Tick(float InDeltaTime) override;

	virtual void Jump() override;

	void LeftAttack();
	void RightAttack();

	void RollStart();
	void TakeDownStart();
	void EquipSword();

	bool Punch1Start();
	bool Punch2Start();
	bool SwordAttack1Start();
	bool SwordAttack2Start();
	bool SwordAttack3Start();

private:
	// 발동 대기중 스킬 초기화
	void _ResetReadySkill();

	// 발동 대기중 스킬 설정
	void _SetReadySkill( function<void()> InReadySkillFunc );

	// 발동 대기중인 스킬 수행
	void _ProcessReadySkill( float InDeltaTime );
};
