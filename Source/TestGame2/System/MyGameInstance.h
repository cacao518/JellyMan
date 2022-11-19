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

	// �ڽ��� �÷��̾ ��ȯ�Ѵ�.
	ACharacterPC* GetMyPlayer();

	// �ڽ��� ��Ʈ�ѷ��� ��ȯ�Ѵ�.
	APlayerController* GetMyController();

	// �ν��Ͻ��� ��ȯ�Ѵ�.
	static UMyGameInstance* GetInstance();
};

inline UMyGameInstance& GetMyGameInstance() 
{ 
	return *( UMyGameInstance::GetInstance() );
};