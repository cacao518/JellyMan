#pragma once

#include "../ETC/SDB.h"


namespace UtilMaterial
{
	// Actor가 가지고 있는 MaterialInterface를 알아낸다.
	UMaterialInterface* GetMatrialInterface( AActor* InOtherActor );

	// 머터리얼 애셋 주소를 EMaterialState로 바꿔준다.
	EMaterialState ConvertMatAssetToMatState( UMaterialInterface* InMaterial );
}