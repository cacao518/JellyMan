// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../System/MonsterAIController.h"
#include "Components/BoxComponent.h"

// Sets default values
AMonster::AMonster()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterAIController = CreateDefaultSubobject<AMonsterAIController>( TEXT( "MonsterAIController" ) );
	if( MonsterAIController )
	{
		AIControllerClass = Cast<UClass>( MonsterAIController );
		MonsterAIController->SetBB( BBAsset );
		MonsterAIController->SetBT( BTAsset );
	}

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GameObject
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );
	GameObject->SetHp       ( Const::DUMMY_HP             );
	GameObject->SetMoveSpeed( Const::DUMMY_MOVE_SPEED     );
	GameObject->SetAttackSpeed( Const::DUMMY_ATTACK_SPEED );
	GameObject->SetJumpPower( Const::DUMMY_JUMP_POWER     );

	// Create a MaterialProperty Component
	MatProperty = CreateDefaultSubobject<UMaterialProperty>( TEXT( "MatProperty" ) );

	// Box Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( GetMesh() );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

