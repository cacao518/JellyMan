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
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 회전을 부드럽게 하기 위한 코드
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// GameObject
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );

	// Create a MaterialProperty Component
	MatProperty = CreateDefaultSubobject<UMaterialProperty>( TEXT( "MatProperty" ) );

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, 1000.0f, 0.0f );

	// Box Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( GetMesh() );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );

	// AttackBox Component
	AttackColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "AttackColl" ) );
	AttackColl->SetupAttachment( GetMesh() );
	AttackColl->SetCollisionProfileName( TEXT( "AttackColl" ) );
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

