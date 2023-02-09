#pragma once

#include "../ETC/SDB.h"


namespace UtilMaterial
{
	// Actor �� �ؿ� �ִ� MaterialInterface�� �˾Ƴ���.
	UMaterialInterface* GetSteppedMatrialInterface( AActor* InActor );

	// Actor�� ������ �ִ� MaterialInterface�� �˾Ƴ���.
	UMaterialInterface* GetMatrialInterface( AActor* InActor );

	// MaterialInterface�� EMaterialState�� �ٲ��ش�.
	EMaterialState ConvertMatAssetToMatState( UMaterialInterface* InMaterial );
}