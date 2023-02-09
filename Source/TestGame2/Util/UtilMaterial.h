#pragma once

#include "../ETC/SDB.h"


namespace UtilMaterial
{
	// Actor�� ������ �ִ� MaterialInterface�� �˾Ƴ���.
	UMaterialInterface* GetMatrialInterface( AActor* InOtherActor );

	// ���͸��� �ּ� �ּҸ� EMaterialState�� �ٲ��ش�.
	EMaterialState ConvertMatAssetToMatState( UMaterialInterface* InMaterial );
}