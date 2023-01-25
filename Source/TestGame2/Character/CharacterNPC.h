// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterNPC.generated.h"


class UAnimMontage;


UCLASS()
class TESTGAME2_API ACharacterNPC final : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	class UCharacterComp* CharacterComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* HitColl;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
	class UBoxComponent* AttackColl;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	TArray<int> SkillInfos;             

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	float PatrolRange;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay, meta = ( AllowPrivateAccess = "true" ) )
	float DetectRange;

public:
	// Sets default values for this character's properties
	ACharacterNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
