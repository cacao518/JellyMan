// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/ObjectComp.h"
#include "Components/BoxComponent.h"

// Sets default values
AGroundObject::AGroundObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 회전을 부드럽게 하기 위한 코드
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ObjectComp
	ObjectComp = CreateDefaultSubobject<UObjectComp>( TEXT( "ObjectComp" ) );

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
void AGroundObject::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AGroundObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

