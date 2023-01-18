// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CameraShake.h"
#include "../Manager/CameraManager.h"

FString UAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CameraShake::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	GetCameraManager().CameraShake( MeshComp->GetOwner(), Scale, ShakeByWeight, ShakeByIntensity );
}