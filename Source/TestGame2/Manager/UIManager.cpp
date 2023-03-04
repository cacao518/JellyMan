
#include "UIManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "../Component/ObjectComp.h"
#include "../Component/MaterialComp.h"
#include "../Actor/CharacterPC.h"
#include "../System/MyAnimInstance.h"
#include "../System/MyGameInstance.h"
#include "../System/TestGame2GameMode.h"
#include "../Manager/LockOnManager.h"
#include "../Manager/DataInfoManager.h"
#include "../ETC/CameraShakeEffect.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"


UIManager::UIManager()
{
	
}

UIManager::~UIManager()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 틱 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::Tick( float InDeltaTime )
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief UI를 생성하고 띄운다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
UUserWidget* UIManager::ShowUI( const FString& InPath )
{
	FString path = FString( TEXT( "/Game/UI/" ) ) + InPath;
	UClass* loadedClass = ConstructorHelpersInternal::FindOrLoadClass( path, UUserWidget::StaticClass() );
	if( !loadedClass )
		return nullptr;

	UUserWidget* userWidget = CreateWidget<UUserWidget>( &GetMyGameInstance(), loadedClass );
	if( !userWidget )
		return nullptr;

	userWidget->AddToViewport( 9999 ); // Z-order, this just makes it render on the very top.

	return userWidget;
}