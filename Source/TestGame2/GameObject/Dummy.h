// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dummy.generated.h"


UCLASS()
class TESTGAME2_API ADummy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = GameObject, meta = ( AllowPrivateAccess = "true" ) )
	class UGameObject* GameObject;

	UPROPERTY( Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

public:
	// Sets default values for this character's properties
	ADummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
