
#include "CameraManager.h"
#include "Engine/World.h"
#include "../Component/GameObject.h"
#include "../Character/CharacterPC.h"
#include "../System/MyAnimInstance.h"
#include "../System/MyGameInstance.h"
#include "../System/TestGame2GameMode.h"
#include "../Manager/LockOnManager.h"
#include "../ETC/CameraShakeEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"


CameraManager::CameraManager()
{
	
}

CameraManager::~CameraManager()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 틱 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CameraManager::Tick( float InDeltaTime )
{
	_ProcessCameraArm( InDeltaTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 카메라 쉐이크를 실행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CameraManager::CameraShake( float InScale, bool InShakeByWeight )
{
	ACharacterPC* player = GetMyGameInstance().GetMyPlayer();
	if( !player )
		return;
	
	APlayerController* controller = GetMyGameInstance().GetMyController();
	if( !controller )
		return;

	auto gameObject = player ? Cast<UGameObject>( player->FindComponentByClass<UGameObject>() ) : nullptr;
	if( !gameObject )
		return;

	if( InShakeByWeight )
	{
		auto moveComponent = player->GetCharacterMovement();
		if( moveComponent )
		{
			if( !moveComponent->IsFalling() && gameObject->GetStat().JumpPower <= 0.5f )
			{
				controller->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), InScale );
			}
		}
	}
	else
	{
		controller->ClientStartCameraShake( UCameraShakeEffect::StaticClass(), InScale );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 카메라 암 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CameraManager::_ProcessCameraArm( float InDeltaTime )
{
	ACharacterPC* player = GetMyGameInstance().GetMyPlayer();
	if( !player )
		return;

	UMyAnimInstance* animInstance = Cast< UMyAnimInstance >( player->GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	USpringArmComponent* cameraBoom = player->CameraBoom;
	if( !cameraBoom )
		return;

	if( animInstance->IsFly )
		cameraBoom->TargetArmLength = FMath::Lerp( cameraBoom->TargetArmLength, Const::FLY_TARGET_ARM_LENGTH, InDeltaTime * 10.f );
	else
		cameraBoom->TargetArmLength = FMath::Lerp( cameraBoom->TargetArmLength, Const::DEFAULT_TARGET_ARM_LENGTH, InDeltaTime * 5.f );

	if( GetLockOnManager().GetLockOnTarget() )
		cameraBoom->CameraRotationLagSpeed = Const::LOCKON_CAMERA_ROTAION_LAG_SPEED;
	else
		cameraBoom->CameraRotationLagSpeed = 0.f;
}