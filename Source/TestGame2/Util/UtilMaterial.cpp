
#include "UtilMaterial.h"
#include "../Manager/DataInfoManager.h"

namespace UtilMaterial
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// @brief Actor�� ������ �ִ� MaterialInterface�� �˾Ƴ���.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	UMaterialInterface* GetMatrialInterface( AActor* InOtherActor )
	{
		UMaterialInterface* matInterface = nullptr;

		// ���ͳ� �÷��̾�
		if ( Cast<ACharacter>( InOtherActor ) )
		{
			/*auto meshOnTile = Cast<ACharacter>( OtherActor )->GetMesh();
			if( !meshOnTile )
				return;

			MatStateOnTile = _ConvertMatAssetToMatState( meshOnTile->GetMaterial( 0 ) );*/
		}
		else
		{
			// ����ƽ �Ž�
			auto staticMesh = Cast<UStaticMeshComponent>( InOtherActor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) );
			if ( staticMesh )
				matInterface = staticMesh->GetMaterial( 0 );

			// ���� �ٵ�
			auto waterBody = Cast<UWaterBodyComponent>( InOtherActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
			if ( waterBody )
			{
				const auto& waterMatInfo = GetDataInfoManager().GetMaterialInfos().Find( EMaterialState::WATER );
				if ( waterMatInfo )
					matInterface = waterMatInfo->MaterialAsset[ 0 ];
			}

			// ���彺������
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
	//// @brief EMaterialState�� ���͸��� �ּ� �ּҷ� �ٲ��ش�.
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