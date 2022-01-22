// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/GameObject.h"
#include "Components/BoxComponent.h"

// Sets default values
ADummy::ADummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GameObject
	GameObject = CreateDefaultSubobject<UGameObject>( TEXT( "GameObject" ) );
	GameObject->AnimState = EAnimState::IDLE_RUN;
	GameObject->MoveSpeed = Const::PLAYER_DEFAULT_SPEED;
	GetCharacterMovement()->MaxWalkSpeed = GameObject->MoveSpeed;

	// Box Component
	HitBox = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitBox" ) );
	HitBox->SetupAttachment( GetMesh() );
	HitBox->SetCollisionProfileName( TEXT( "Character" ) );
	HitBox->SetBoxExtent( FVector( 20.f, 20.f, 20.f ) );
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

