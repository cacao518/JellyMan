// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ETC/SDB.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttackStart.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UAnimNotify_AttackStart : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = ( ExposeOnSpawn = true ) )
	FVector Size;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = ( ExposeOnSpawn = true ) )
	FVector Pos;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = ( ExposeOnSpawn = true ) )
	float Power;

public:
	virtual FString GetNotifyName_Implementation() const override;

private:
	void Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation ) override;
	
};
