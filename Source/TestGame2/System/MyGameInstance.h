// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/GameEngine.h"
#include "MyGameInstance.generated.h"


UCLASS()
class TESTGAME2_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	~UMyGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
	void Tick( float DeltaTime );

	static UMyGameInstance* GetInstance();
};

inline UMyGameInstance& GetMyGameInstance() 
{ 
	return *( UMyGameInstance::GetInstance() );
};