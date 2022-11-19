// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/GameEngine.h"
#include "MyGameInstance.generated.h"


class ACharacterPC;


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

	// 자신의 플레이어를 반환한다.
	ACharacterPC* GetMyPlayer();

	// 자신의 컨트롤러를 반환한다.
	APlayerController* GetMyController();

	// 인스턴스를 반환한다.
	static UMyGameInstance* GetInstance();
};

inline UMyGameInstance& GetMyGameInstance() 
{ 
	return *( UMyGameInstance::GetInstance() );
};