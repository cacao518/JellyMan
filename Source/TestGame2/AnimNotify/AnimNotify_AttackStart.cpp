// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackStart.h"
#include "../Component/GameObject.h"

FString UAnimNotify_AttackStart::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_AttackStart::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	UGameObject* obj = Cast<UGameObject>( MeshComp->GetOwner()->FindComponentByClass<UGameObject>() );
	if( !obj ) return;

	obj->SetAttackCollInfo( FCollisionInfo( Size, Pos, Power, KnockBackPower ) );
	obj->SetIsEnabledAttackColl( true );
}
