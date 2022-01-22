// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UGameObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGameObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetIsAttackMove() { return IsAttackMove; };
	void SetIsAttackMove( bool InIsAttackMove ){ IsAttackMove = InIsAttackMove; };
	void SetAttackCollSize( float InX = 0.f, float InY = 0.f, float InZ = 0.f );
	void SetAttackCollSize( const FVector& InVector );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EAnimState AnimState;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	FVector AttackCollSize;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	float      MoveSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	bool       IsAttackMove;

private:
	void _AnimStateChange();
	void _Move();
	void _ResetAttackColl();
};
