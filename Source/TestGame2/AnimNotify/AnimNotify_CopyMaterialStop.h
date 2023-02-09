// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CopyMaterialStop.generated.h"

/**
 *  �ش� �ڵ� ���� Ʈ���̽� ������� ���� ���� ��� ����
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
