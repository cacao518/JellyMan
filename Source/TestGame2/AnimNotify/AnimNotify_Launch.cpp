// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Launch.h"
#include "../ETC/SDB.h"
#include "../Component/CharacterComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UAnimNotify_Launch::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Launch::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp )
		return;

	ACharacter* owner = Cast<ACharacter>( MeshComp->GetOwner() );
	if( !owner )
		return;

	UCharacterComp* obj = Cast<UCharacterComp>( MeshComp->GetOwner()->FindComponentByClass<UCharacterComp>() );
	if ( !obj )
		return;
	
	const FVector vec = ( Const::MAX_MASS - obj->GetStat().Weight ) * Vec;
	owner->LaunchCharacter( vec, true, true );
}