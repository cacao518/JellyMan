
#include "UtilMaterial.h"
#include "Engine/World.h"
#include "WaterBodyComponent.h"
#include "Landscape.h"
#include "GameFramework/Character.h"
#include "../System/MyGameInstance.h"
#include "../Manager/DataInfoManager.h"

namespace UtilMaterial
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @briefActor 발 밑에 있는 MaterialInterface을 알아낸다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	UMaterialInterface* GetSteppedMatrialInterface( AActor* InActor )
	{
		FVector LineTraceStart = InActor->GetActorLocation();
		FVector LineTraceEnd = FVector( InActor->GetActorLocation().X, InActor->GetActorLocation().Y, InActor->GetActorLocation().Z - 150.f );

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
		FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, InActor ); //Tag, Bool Trace Complex, Ignore 액터 (자신 제외)
		FHitResult hitResult;
		GetMyGameInstance().GetWorld()->LineTraceSingleByObjectType(
			OUT hitResult,
			LineTraceStart,
			LineTraceEnd,
			FCollisionObjectQueryParams( ECollisionChannel::ECC_WorldStatic ),
			TraceParameters
		);

		AActor* hitActor = hitResult.GetActor();
		if ( hitActor )
		{
			//FString str = InActor->GetName() + TEXT( "Set SteppedMatState" );
			//if ( GEngine )
			//	GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

			return GetMatrialInterface( hitActor );
		}

		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @brief Actor가 가지고 있는 MaterialInterface를 알아낸다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	UMaterialInterface* GetMatrialInterface( AActor* InActor )
	{
		UMaterialInterface* matInterface = nullptr;

		// 몬스터나 플레이어
		if ( Cast<ACharacter>( InActor ) )
		{
			/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
			if( !meshOnTile )
				return;

			MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
		}
		else
		{
			// 스테틱 매쉬
			auto staticMesh = Cast<UStaticMeshComponent>( InActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
			if ( staticMesh )
				matInterface = staticMesh->GetMaterial( 0 );

			// 워터 바디
			auto waterBody = Cast<UWaterBodyComponent>( InActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
			if ( waterBody )
			{
				const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::WATER );
				if ( waterMatInfo )
				{
					FString path = waterMatInfo->MaterialAssetPaths[ 0 ];
					matInterface = LoadObject<UMaterialInterface>( NULL, *path, NULL, LOAD_None, NULL );
				}
			}

			// 랜드 스케이프
			auto landScape = Cast<ULandscapeComponent>( InActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
			if( landScape )
			{
				matInterface = landScape->GetMaterial( 0 );
			}

			//auto landScape = Cast<ALandscape>( InActor );
			//if ( landScape )
			//{
			//	/*const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::DEEPWATER );
			//	if ( waterMatInfo )
			//	{
			//		FString path = waterMatInfo->MaterialAssetPaths[ 0 ];
			//		matInterface = LoadObject<UMaterialInterface>( NULL, *path, NULL, LOAD_None, NULL );
			//	}*/
			//}
		}

		return matInterface;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @brief MaterialInterface를 EMaterialState로 바꿔준다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	EMaterialState ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
	{
		if ( !InMaterial )
			return EMaterialState::DEFAULT;

		for ( const auto& [state, matInfo] : GetDataInfoManager().GetMaterialInfos() )
		{
			if ( matInfo.MaterialAssetPaths.Find( InMaterial->GetPathName() ) != INDEX_NONE )
				return state;
		}

		return EMaterialState::DEFAULT;
	}
}