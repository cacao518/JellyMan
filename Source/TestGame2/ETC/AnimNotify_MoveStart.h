// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MoveStart.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UAnimNotify_MoveStart : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = ( ExposeOnSpawn = true ) )
	float MovePower;

public:
	virtual FString GetNotifyName_Implementation() const override;

private:
	void Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation ) override;
	
};
