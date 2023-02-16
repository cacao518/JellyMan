// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_SpawnActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Manager/ObjectManager.h"
#include "../Component/ObjectComp.h"

FString UAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SpawnActor::SetProperty( AActor* InOwner )
{
	if ( !InOwner )
		return;

	UObjectComp* objComp = Cast<UObjectComp>( InOwner->FindComponentByClass<UObjectComp>() );
	if ( !objComp )
		return;

	auto spawnPosComp = Cast<USceneComponent>( InOwner->GetDefaultSubobjectByName( TEXT( "SpawnPosComp" ) ) );
	if ( !spawnPosComp )
		return;

	FVector relativeSpawnPos = spawnPosComp->GetRelativeLocation() + Pos;
	FVector worldSpawnPos = UKismetMathLibrary::TransformLocation( spawnPosComp->GetComponentTransform(), relativeSpawnPos );

	ResultActor  = Actor;
	ResultPos    = worldSpawnPos;
	ResultRotate = InOwner->GetActorRotation() + Rotate;
}

void UAnimNotify_SpawnActor::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	if ( !ObjectManager::IsVaild() )
		return;

	AActor* owner = Cast<AActor>( MeshComp->GetOwner() );
	if ( !owner )
		return;

	UObjectComp* objComp = Cast<UObjectComp>( owner->FindComponentByClass<UObjectComp>() );
	if ( !objComp )
		return;

	SetProperty( owner );

	if( SetAsParentTeamType )
		GetObjectManager().SpawnActor( ResultActor, ResultPos, ResultRotate, objComp->GetTeamType() );
	else
		GetObjectManager().SpawnActor( ResultActor, ResultPos, ResultRotate );
}