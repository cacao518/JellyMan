
#include "LockOnManager.h"
#include "Engine/World.h"
#include "../Component/ObjectComp.h"
#include "../Component/CharacterComp.h"
#include "../Actor/CharacterPC.h"
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
:	 
LockOnTarget ( nullptr )
{

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
void LockOnManager::LockOnStart( ELockOnMode InMode )
{
	UWorld* world = GetMyGameInstance().GetWorld();
	if( !world )
		return;

	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if( !myPlayer )
		return;

	auto myPlayerCharComp = Cast<UCharacterComp>( myPlayer->FindComponentByClass<UCharacterComp>() );
	if( !myPlayerCharComp )
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

	// Character�� ����.
	TArray< ACharacter* > filteredCharacters;
	for ( const auto& result : overlapResults )
	{
		ACharacter* character = Cast< ACharacter >( result.GetActor() );
		if ( !character )
			continue;

		auto charComp = Cast<UCharacterComp>( character->FindComponentByClass<UCharacterComp>() );
		if ( !charComp )
			continue;

		// ���� ���� �����Ѵ�.
		if ( charComp->GetTeamType() == myPlayerCharComp->GetTeamType() )
			continue;

		filteredCharacters.AddUnique( character );
	}

	if ( filteredCharacters.IsEmpty() )
		return;

	ACharacter* selectLockOnTarget = nullptr;

	switch ( InMode )
	{
		case ELockOnMode::Default:
		{
			// ���� ����� ���� ���� �ǵ��� ���� �� ����
			filteredCharacters.Sort( [myPlayer]( const auto& A, const auto& B ){
				return A.GetDistanceTo( myPlayer ) < B.GetDistanceTo( myPlayer );
				} );

			selectLockOnTarget = ( *filteredCharacters.begin() );
			break;
		}
		case ELockOnMode::Prev:
		{
			// ���� �� ��� ���� ������� �ٽ� ����
			int32 index = filteredCharacters.IndexOfByPredicate( [this]( const ACharacter* InChar ){
				return InChar == LockOnTarget;
				} );

			if ( index != INDEX_NONE && filteredCharacters.IsValidIndex( index - 1) )
			{
				selectLockOnTarget = filteredCharacters[ index - 1 ];
				GetLockOnManager().LockOnRelease();
			}
			break;
		}
		case ELockOnMode::Next:
		{
			// ���� �� ��� ���� ������� �ٽ� ����
			int32 index = filteredCharacters.IndexOfByPredicate( [this]( const ACharacter* InChar ){
				return InChar == LockOnTarget;
				} );

			if ( index != INDEX_NONE && filteredCharacters.IsValidIndex( index + 1 ) )
			{
				selectLockOnTarget = filteredCharacters[ index + 1 ];
				GetLockOnManager().LockOnRelease();
			}
			break;
		}
	}

	if ( !selectLockOnTarget )
		return;

	if ( auto widgetComp = Cast<UWidgetComponent>( selectLockOnTarget->GetDefaultSubobjectByName( TEXT( "LockOnMarkComp" ) ) ) )
	{
		if ( auto userWidget = Cast<UUserWidget>( widgetComp->GetUserWidgetObject() ) )
		{
			FOutputDeviceNull ar;
			userWidget->CallFunctionByNameWithArguments( TEXT( "AnimStart" ), ar, NULL, true );
			userWidget->SetVisibility( ESlateVisibility::SelfHitTestInvisible );

			LockOnTarget = selectLockOnTarget;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ����
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LockOnManager::LockOnRelease()
{
	if( auto widgetComp = Cast<UWidgetComponent>( LockOnTarget->GetDefaultSubobjectByName( TEXT( "LockOnMarkComp" ) ) ) )
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

	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if( !myPlayer )
		return;

	auto ownerObjectComp = Cast<UCharacterComp>( myPlayer->FindComponentByClass<UCharacterComp>() );
	if( !ownerObjectComp )
		return;

	auto otherObjectComp = Cast<UObjectComp>( LockOnTarget->FindComponentByClass<UObjectComp>() );
	if( !otherObjectComp )
		return;

	bool targetDie = !GetValid( LockOnTarget ) || otherObjectComp->GetIsDie();
	bool targetAway = LockOnTarget->GetDistanceTo( myPlayer ) > Const::LOCKON_RANGE;
	bool ownerDie = !GetValid( myPlayer ) || ownerObjectComp->GetIsDie();
	
	if ( ownerDie || targetAway )
	{
		LockOnRelease();
		return;
	}

	if( targetDie )
	{
		LockOnRelease();
		LockOnStart(); // ���� �� ���� ���� ���, ��ó ���� �ٽ� ����.
		return;
	}

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( myPlayer->GetActorLocation(), LockOnTarget->GetActorLocation() );
	rotator.Pitch = Const::LOCKON_CAMERA_FIX_PITCH;

	myPlayer->GetController()->SetControlRotation( rotator );
	
	// ���� ���¿��� ��ų ��� �� �� ĳ���Ͱ� ���� �ٶ󺸰� �� �� ���� Ȯ��
	auto skillInfo = ownerObjectComp->GetCurSkillInfo();
	if( skillInfo && skillInfo->LockOnLookAt && ownerObjectComp->IsMontageInitialTime() )
		ownerObjectComp->LookAt( GetLockOnManager().GetLockOnTarget() );
}