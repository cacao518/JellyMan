// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME2_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	float        Speed;                 // 이동속도   

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	bool         IsJump;                // 점프 중 여부

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	bool         IsFly;                 // 날기 중 여부

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	bool         IsDie;                 // 사망 여부

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay )
	EAnimState   AnimState;             // 애니메이션 상태

};
