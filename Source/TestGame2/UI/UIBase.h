// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBase.generated.h"


UCLASS()
class TESTGAME2_API UUIBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// UI가 생성됨을 알린다.
	virtual void OnCreated() {};
};
