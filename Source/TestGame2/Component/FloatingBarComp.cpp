// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingBarComp.h"
#include "Components/ProgressBar.h"
#include "../Actor/CharacterPC.h"
#include "../System/MyAnimInstance.h"
#include "../System/MyGameInstance.h"

UFloatingBarComp::UFloatingBarComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	ShowTime = 0;
}

UFloatingBarComp::~UFloatingBarComp()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UFloatingBarComp::BeginPlay()
{
	Super::BeginPlay();

	UserWidget = Cast<UUserWidget>( GetUserWidgetObject() );
	if( !UserWidget )
		return;

	HpProgressBar = Cast<UProgressBar>( UserWidget->GetWidgetFromName( TEXT( "HpProgressBar" ) ) );

	OwningActor = Cast<AActor>( GetOwner() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UFloatingBarComp::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( ShowTime > 0 )
		ShowTime -= DeltaTime;
	else
		ShowTime = 0;

	if( !UserWidget || ( UserWidget && UserWidget->GetVisibility() == ESlateVisibility::Collapsed ) )
		return;

	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if( !myPlayer )
		return;

	// ���� �ð��� ����ǰ�, �Ÿ��� �־����� HP�ٸ� �����.
	bool targetAway = OwningActor->GetDistanceTo( myPlayer ) > Const::LOCKON_RANGE;
	if( targetAway && ShowTime == 0 )
	{
		if( UserWidget )
			UserWidget->SetVisibility( ESlateVisibility::Collapsed );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief HP�� ������Ʈ �Լ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UFloatingBarComp::UpdateHpBar( float InPercent )
{
	if( !UserWidget || !HpProgressBar )
		return;

	ShowTime = 5.f;

	HpProgressBar->SetPercent( InPercent );

	if( InPercent <= 0 )
		UserWidget->SetVisibility( ESlateVisibility::Collapsed );
	else
		UserWidget->SetVisibility( ESlateVisibility::SelfHitTestInvisible );
}