
#include "LockOnManager.h"
#include "Engine/World.h"
#include "../Component/GameObject.h"
#include "../Character/CharacterPC.h"
#include "../System/MyAnimInstance.h"
#include "../System/MyGameInstance.h"
#include "../System/TestGame2GameMode.h"
#include "../ETC/SDB.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"


LockOnManager::LockOnManager()
{
	LockOnTarget = nullptr;
}

LockOnManager::~LockOnManager()
{
	LockOnTarget = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 틱 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::Tick( float InDeltaTime )
{
	_ProcessLockOn();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 시작
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::LockOnStart()
{
	UWorld* world = GetMyGameInstance().GetWorld();
	if( !world )
		return;

	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if( !myPlayer )
		return;

	FVector center = myPlayer->GetActorLocation();

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam( NAME_None, false, myPlayer );
	bool bResult = world->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere( Const::LOCKON_RANGE ),
		collisionQueryParam
	);

	if( !bResult )
		return;

	// 제일 가까운 적이 락온 되도록 정렬
	overlapResults.Sort( [myPlayer]( const auto& A, const auto& B ){
		return A.GetActor()->GetDistanceTo( myPlayer ) < B.GetActor()->GetDistanceTo( myPlayer );
		} );

	for( FOverlapResult overlapResult : overlapResults )
	{
		ACharacter* character = Cast<ACharacter>( overlapResult.GetActor() );
		if( !character )
			continue;

		if( auto widgetComp = Cast<UWidgetComponent>( character->GetDefaultSubobjectByName( TEXT( "LockOnMark" ) ) ) )
		{
			if( auto userWidget = Cast<UUserWidget>( widgetComp->GetUserWidgetObject() ) )
			{
				FOutputDeviceNull ar;
				userWidget->CallFunctionByNameWithArguments( TEXT( "AnimStart" ), ar, NULL, true );
				userWidget->SetVisibility( ESlateVisibility::SelfHitTestInvisible );

				LockOnTarget = character;
				return;
			}
		}
	}

	LockOnTarget = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 해제
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::LockOnRelease()
{
	if( auto widgetComp = Cast<UWidgetComponent>( LockOnTarget->GetDefaultSubobjectByName( TEXT( "LockOnMark" ) ) ) )
	{
		if( auto userWidget = Cast<UUserWidget>( widgetComp->GetUserWidgetObject() ) )
			userWidget->SetVisibility( ESlateVisibility::Collapsed );
	}

	LockOnTarget = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 락온 기능 수행
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::_ProcessLockOn()
{
	if( !LockOnTarget )
		return;

	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if( !myPlayer )
		return;

	auto otherGameObject = Cast<UGameObject>( LockOnTarget->FindComponentByClass<UGameObject>() );
	if( !otherGameObject )
		return;

	if( !GetValid( LockOnTarget ) || otherGameObject->GetAnimState() == EAnimState::DIE || LockOnTarget->GetDistanceTo( myPlayer ) > Const::LOCKON_RANGE )
	{
		LockOnRelease();
		return;
	}

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( myPlayer->GetActorLocation(), LockOnTarget->GetActorLocation() );
	rotator.Pitch = myPlayer->GetControlRotation().Pitch;
	myPlayer->GetController()->SetControlRotation( rotator );
}