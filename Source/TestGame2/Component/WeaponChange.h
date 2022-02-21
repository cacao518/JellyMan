// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ETC/SDB.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponChange.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTGAME2_API UWeaponChange : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	ACharacter*            OwningCharacter;             // �θ� ĳ���� Ŭ����

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	EWeaponState           WeaponState;       	        // ���� ����

	class UStaticMeshComponent*     CurWeaponMesh;               // ���� ���� ����ƽ�Ž�
	TArray< UStaticMeshComponent* > WeaponMeshes;                // ���� ����ƽ �޽� �ּµ�

	class UMaterialInstanceDynamic* DissovleMaterialInstance;    // ������ ��Ƽ���� �ν��Ͻ�
	UMaterialInterface*             DissolveMaterial;            // ������ ��Ƽ���� �ּ�
	float                           DissolveAmount;              // ������ ��ȯ ��

public:	
	UWeaponChange();

	// Begin �Լ�
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	
	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// ���⸦ �����Ѵ�.
	void SetWeaponState( EWeaponState InWeaponState, bool InChangeAnim = false );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// ���� ���� ���¸� ��ȯ�Ѵ�.
	EWeaponState GetWeaponState() { return WeaponState; };

private:
	// ������ �ִϸ��̼��� ���� ó���Ѵ�.
	void _DissovleAnimEnd();

};
