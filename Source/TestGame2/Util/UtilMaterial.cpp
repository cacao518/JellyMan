
#include "UtilMaterial.h"
#include "Engine/World.h"
#include "../System/MyGameInstance.h"
#include "../Manager/DataInfoManager.h"

namespace UtilMaterial
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @briefActor �� �ؿ� �ִ� MaterialInterface�� �˾Ƴ���.
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

		// ���� ���� ����
		FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, InActor ); //Tag, Bool Trace Complex, Ignore ���� (�ڽ� ����)
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
	//// @brief Actor�� ������ �ִ� MaterialInterface�� �˾Ƴ���.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	UMaterialInterface* GetMatrialInterface( AActor* InActor )
	{
		UMaterialInterface* matInterface = nullptr;

		// ���ͳ� �÷��̾�
		if ( Cast<ACharacter>( InActor ) )
		{
			/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
			if( !meshOnTile )
				return;

			MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
		}
		else
		{
			// ����ƽ �Ž�
			auto staticMesh = Cast<UStaticMeshComponent>( InActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
			if ( staticMesh )
				matInterface = staticMesh->GetMaterial( 0 );

			// ���� �ٵ�
			auto waterBody = Cast<UWaterBodyComponent>( InActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
			if ( waterBody )
			{
				const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::WATER );
				if ( waterMatInfo )
					matInterface = waterMatInfo->MaterialAsset[ 0 ];
			}

			// ���彺������
			auto landScape = Cast<ULandscapeComponent>( InActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
			if ( landScape )
			{
				auto proxy = landScape->GetLandscapeProxy();
				if ( !proxy )
				{
					if ( GEngine )
						GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Blue, "proxy is null" );
					return nullptr;
				}

				matInterface = proxy->GetLandscapeMaterial( 0 );
			}
		}

		return matInterface;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @brief MaterialInterface�� EMaterialState�� �ٲ��ش�.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	EMaterialState ConvertMatAssetToMatState( UMaterialInterface* InMaterial )
	{
		for ( const auto& [state, matInfo] : GetDataInfoManager().GetMaterialInfos() )
		{
			if ( matInfo.MaterialAsset.Find( InMaterial ) != INDEX_NONE )
				return state;
		}

		return EMaterialState::MAX;
	}
}