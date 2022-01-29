// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestGame2Character.generated.h"


class UAnimMontage;
class USkeletalMeshComponent;


UCLASS(config=Game)
class ATestGame2Character : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = GameObject, meta = ( AllowPrivateAccess = "true" ) )
	class UGameObject* GameObject;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* AttackColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	float BaseTurnRate;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	float BaseLookUpRate;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage* RollAnimation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage* TakeDownAnimation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage* Punch1Animation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage* Punch2Animation;

public:
	ATestGame2Character();

	virtual void BeginPlay() override;

	virtual void Tick(float InDeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void Jump() override;

private:
	void _RollStart();
	void _Punch1Start();
	void _Punch2Start();
	void _TakeDownStart();
};

