// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticObject.h"
#include "../Component/ObjectComp.h"
#include "Components/BoxComponent.h"

// Sets default values
AStaticObject::AStaticObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootComp" ) );

	// ObjectComp
	ObjectComp = CreateDefaultSubobject<UObjectComp>( TEXT( "ObjectComp" ) );

	// Box Component
	HitColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "HitColl" ) );
	HitColl->SetupAttachment( RootComponent );
	HitColl->SetCollisionProfileName( TEXT( "HitColl" ) );

	// AttackBox Component
	AttackColl = CreateDefaultSubobject<UBoxComponent>( TEXT( "AttackColl" ) );
	AttackColl->SetupAttachment( RootComponent );
	AttackColl->SetCollisionProfileName( TEXT( "AttackColl" ) );
}
