
#include "LockOnManager.h"
#include "Engine/World.h"
#include "../Component/GameObject.h"
#include "../Character/GamePlayer.h"
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
//// @brief ƽ �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::Tick( float InDeltaTime )
{
	_ProcessLockOn();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::LockOnStart()
{
	UWorld* world = GetMyGameInstance().GetWorld();
	if( !world )
		return;

	AGamePlayer* myPlayer = GetMyGameInstance().GetMyPlayer();
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

	// ���� ����� ���� ���� �ǵ��� ����
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
//// @brief ���� ����
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
//// @brief ���� ��� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::_ProcessLockOn()
{
	if( !LockOnTarget )
		return;

	AGamePlayer* myPlayer = GetMyGameInstance().GetMyPlayer();
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