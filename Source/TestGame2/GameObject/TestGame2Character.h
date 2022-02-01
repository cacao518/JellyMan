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

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = GameObject, meta = ( AllowPrivateAccess = "true" ) )
	class UMaterialProperty* MatProperty;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* AttackColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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

	virtual void Jump() override;

	void RollStart();
	void Punch1Start();
	void Punch2Start();
	void TakeDownStart();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

