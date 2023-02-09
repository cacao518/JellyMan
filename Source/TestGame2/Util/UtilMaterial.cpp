
#include "UtilMaterial.h"
#include "../Manager/DataInfoManager.h"

namespace UtilMaterial
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @brief Actor가 가지고 있는 MaterialInterface를 알아낸다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	UMaterialInterface* GetMatrialInterface( AActor* InOtherActor )
	{
		UMaterialInterface* matInterface = nullptr;

		// 몬스터나 플레이어
		if ( Cast<ACharacter>( InOtherActor ) )
		{
			/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
			if( !meshOnTile )
				return;

			MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
		}
		else
		{
			// 스테틱 매쉬
			auto staticMesh = Cast<UStaticMeshComponent>( InOtherActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
			if ( staticMesh )
				matInterface = staticMesh->GetMaterial( 0 );

			// 워터 바디
			auto waterBody = Cast<UWaterBodyComponent>( InOtherActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
			if ( waterBody )
			{
				const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::WATER );
				if ( waterMatInfo )
					matInterface = waterMatInfo->MaterialAsset[ 0 ];
			}

			// 랜드스케이프
			auto landScape = Cast<ULandscapeComponent>( InOtherActor->GetComponentByClass( ULandscapeComponent::StaticClass() ) );
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
	//// @brief EMaterialState를 머터리얼 애셋 주소로 바꿔준다.
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