// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/GameObject.h"
#include "../Component/MaterialProperty.h"
#include "../System/MonsterAIController.h"
#include "Components/BoxComponent.h"

// Sets default values
ACharacterNPC::ACharacterNPC()
{
	//AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ȸ���� �ε巴�� �ϱ� ���� �ڵ�
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// GameObject
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f );

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
void ACharacterNPC::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ACharacterNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
