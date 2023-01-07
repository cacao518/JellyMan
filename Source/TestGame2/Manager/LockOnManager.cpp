
#include "LockOnManager.h"
#include "Engine/World.h"
#include "../Component/ObjectComp.h"
#include "../Component/CharacterComp.h"
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
:	 
LockOnTarget ( nullptr )
{

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

	// 제일 가까운 적이 락온 되도록 정렬
	overlapResults.Sort( [myPlayer]( const auto& A, const auto& B ){
		return A.GetActor()->GetDistanceTo( myPlayer ) < B.GetActor()->GetDistanceTo( myPlayer );
		} );

	for( const FOverlapResult& overlapResult : overlapResults )
	{
		ACharacter* character = Cast<ACharacter>( overlapResult.GetActor() );
		if( !character )
			continue;

		auto charComp = Cast<UCharacterComp>( character->FindComponentByClass<UCharacterComp>() );
		if( !charComp )
			continue;

		if( charComp->GetTeamType() == myPlayerCharComp->GetTeamType() )
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

	auto ownerObjectComp = Cast<UCharacterComp>( myPlayer->FindComponentByClass<UCharacterComp>() );
	if( !ownerObjectComp )
		return;

	auto otherObjectComp = Cast<UObjectComp>( LockOnTarget->FindComponentByClass<UObjectComp>() );
	if( !otherObjectComp )
		return;

	bool targetDie = !GetValid( LockOnTarget ) || otherObjectComp->GetIsDie();
	bool targetAway = LockOnTarget->GetDistanceTo( myPlayer ) > Const::LOCKON_RANGE;
	bool ownerDie = !GetValid( myPlayer ) || ownerObjectComp->GetIsDie();

	if( targetDie || targetAway || ownerDie )
	{
		LockOnRelease();
		return;
	}

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( myPlayer->GetActorLocation(), LockOnTarget->GetActorLocation() );
	rotator.Pitch = myPlayer->GetControlRotation().Pitch;
	myPlayer->GetController()->SetControlRotation( rotator );
	
	// 락온 상태에서 스킬 사용 시 내 캐릭터가 적을 바라보게 할 것 인지 확인
	auto skillInfo = ownerObjectComp->GetCurSkillInfo();
	if( skillInfo && skillInfo->LockOnLookAt && ownerObjectComp->IsMontageInitialTime() )
		ownerObjectComp->LookAt( GetLockOnManager().GetLockOnTarget() );
}