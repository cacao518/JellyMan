// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_FootStep.h"
#include "../ETC/SDB.h"
#include "../Component/CharacterComp.h"
#include "../Manager/ObjectManager.h"
#include "../Util/UtilMaterial.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UAnimNotify_FootStep::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_FootStep::Notify( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if ( !MeshComp )
		return;

	ACharacter* owner = Cast<ACharacter>( MeshComp->GetOwner() );
	if ( !owner )
		return;

	auto spawnPosComp = Cast<USceneComponent>( MeshComp->GetOwner()->GetDefaultSubobjectByName( TEXT( "SpawnPosComp" ) ) );
	if ( !spawnPosComp )
		return;

	FVector relativeSpawnPos = spawnPosComp->GetRelativeLocation() + FootLocation;
	FVector worldSpawnPos = UKismetMathLibrary::TransformLocation( spawnPosComp->GetComponentTransform(), relativeSpawnPos );


	switch ( _GetMatState( owner ) )
	{
		case EMaterialState::WATER :
		{
			GetObjectManager().SpawnParticle( TEXT( "FootSteps/FootStep_Water" ), owner, worldSpawnPos, owner->GetActorRotation() );
			break;
		}
	}
}

EMaterialState UAnimNotify_FootStep::_GetMatState( ACharacter* InOwner )
{
	FVector LineTraceStart = InOwner->GetActorLocation();
	FVector LineTraceEnd = FVector( InOwner->GetActorLocation().X, InOwner->GetActorLocation().Y, InOwner->GetActorLocation().Z - 150.f );

	/*DrawDebugLine(
		GetWorld(),
		LineTraceStart,
		LineTraceEnd,
		FColor( 255, 0, 0 ),
		false,
		0.f,
		0.f,
		10.f
	);*/

	// 쿼리 변수 설정
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, InOwner ); //Tag, Bool Trace Complex, Ignore 액터 (자신 제외)
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_WorldStatic ),
		TraceParameters
	);

	AActor* hitActor = hitResult.GetActor();
	if ( hitActor )
	{
		FString str = InOwner->GetName() + TEXT( "Set SteppedMatState" );
		if ( GEngine )
			GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

		UMaterialInterface* matInterface = UtilMaterial::GetMatrialInterface( hitActor );
		return UtilMaterial::ConvertMatAssetToMatState( matInterface );
	}

	return EMaterialState::MAX;
}