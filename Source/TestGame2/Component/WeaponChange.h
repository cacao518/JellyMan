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
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Gameplay )
	EWeaponState                    WeaponState;       	         // 무기 상태

private:
	ACharacter*                     OwningCharacter;             // 부모 캐릭터 클래스
	class UStaticMeshComponent*     CurWeaponMesh;               // 현재 무기 스태틱매쉬
	TArray< UStaticMeshComponent* > WeaponMeshes;                // 무기 스태틱 메시 애셋들

	class UMaterialInstanceDynamic* DissovleMaterialInstance;    // 디졸브 머티리얼 인스턴스
	UMaterialInterface*             DissolveMaterial;            // 디졸브 머티리얼 주소
	float                           DissolveAmount;              // 디졸브 변환 값

public:	
	UWeaponChange();

	// Begin 함수
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	
	///////////////////////////////////////////////////////////////////////
	// Setter
	///////////////////////////////////////////////////////////////////////
	
	// 무기를 변경한다.
	void SetWeaponState( EWeaponState InWeaponState, bool InChangeAnim = true );

	///////////////////////////////////////////////////////////////////////
	// Getter
	///////////////////////////////////////////////////////////////////////

	// 현재 무기 상태를 반환한다.
	EWeaponState GetWeaponState() { return WeaponState; };

	// 현재 무기 스태틱매쉬를 반환한다.
	UStaticMeshComponent* GetCurWeaponMesh() { return CurWeaponMesh; };

private:
	// 무기 메쉬 주소를 저장해놓는다.
	void _InitWeaponMesh();

	// 디졸브 애니메이션을 종료 처리한다.
	void _DissovleAnimEnd();

};
