// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/CharacterComp.h"
#include "../Component/MaterialComp.h"
#include "../System/MonsterAIController.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ACharacterNPC::ACharacterNPC()
{
	//AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 회전을 부드럽게 하기 위한 코드
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// CharacterComp
	CharacterComp = CreateDefaultSubobject<UCharacterComp>( TEXT( "CharacterComp" ) );

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, Const::DEFAULT_ROTATION_RATE, 0.0f );

	// SpawnPosComp Component
	SpawnPosComp = CreateDefaultSubobject<USceneComponent>( TEXT( "SpawnPosComp" ) );
	SpawnPosComp->SetupAttachment( RootComponent );

	// Box Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( GetMesh() );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );

	// AttackBox Component
	AttackColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "AttackColl" ) );
	AttackColl->SetupAttachment( GetMesh() );
	AttackColl->SetCollisionProfileName( TEXT( "AttackColl" ) );
}