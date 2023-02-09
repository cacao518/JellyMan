// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CopyMaterialStop.h"
#include "../Component/MaterialComp.h"

/**
 *  해당 코드 라인 트레이스 방식으로 변경 이후 사용 안함
 */
FString UAnimNotify_CopyMaterialStop::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_CopyMaterialStop::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if( !MeshComp || !( MeshComp->GetOwner() ) )
		return;

	UMaterialComp* matProperty = Cast<UMaterialComp>( MeshComp->GetOwner()->FindComponentByClass<UMaterialComp>() );
	if( !matProperty ) return;
}