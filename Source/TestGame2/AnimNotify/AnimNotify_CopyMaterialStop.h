// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CopyMaterialStop.generated.h"

/**
 *  해당 코드 라인 트레이스 방식으로 변경 이후 사용 안함
 */
UCLASS()
class TESTGAME2_API UAnimNotify_CopyMaterialStop : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

private:
	void Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation ) override;
	
};
