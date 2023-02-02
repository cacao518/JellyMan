// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectComp.h"
#include "MaterialComp.h"
#include "WeaponComp.h"
#include "../ETC/SDB.h"
#include "../Character/CharacterPC.h"
#include "../Character/CharacterNPC.h"
#include "../Character/StaticObject.h"
#include "../Character/Projectile.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/DataInfoManager.h"
#include "../Manager/CameraManager.h"
#include "../Manager/LockOnManager.h"
#include "../System/MonsterAIController.h"
#include "../System/MyGameInstance.h"
#include "../System/MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "WaterBodyComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
#include "Kismet/KismetMathLibrary.h"


UObjectComp::UObjectComp()
:
OwningActor           ( nullptr ),
TeamType              ( ETeamType::MAX ),
IsDie                 ( false   ),
IsFallWater           ( false   ),
IsEnabledAttackColl   ( false   )
{
	PrimaryComponentTick.bCanEverTick = true;
}

UObjectComp::~UObjectComp()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::BeginPlay()
{
	Super::BeginPlay();

	_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_ProcessDie();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 대상을 바라본다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::LookAt( ACharacter* InTarget )
{
	if( !OwningActor || !InTarget )
		return;

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation( OwningActor->GetActorLocation(), InTarget->GetActorLocation() );
	rotator.Pitch = OwningActor->GetActorRotation().Pitch;
	OwningActor->SetActorRotation( rotator );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 콜리전 정보를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::SetAttackCollInfo( const FCollisionInfo& InAttackCollInfo )
{
	AttackCollInfo = InAttackCollInfo;

	auto attackColl = OwningActor ? Cast<UBoxComponent>( OwningActor->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
	{ 
		attackColl->SetBoxExtent( AttackCollInfo.Size );
		attackColl->SetRelativeLocation( AttackCollInfo.Pos );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동속도를 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::SetMoveSpeed( float InMoveSpeed )
{
	Stat.MoveSpeed = InMoveSpeed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 점프력을 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::SetJumpPower( float InJumpPower )
{
	Stat.JumpPower = InJumpPower;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 콜리전 활성화 여부를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::SetIsEnabledAttackColl( bool InIsEnabledAttackColl )
{
	IsEnabledAttackColl = InIsEnabledAttackColl;

	auto attackColl = OwningActor ? Cast<UBoxComponent>( OwningActor->GetDefaultSubobjectByName( TEXT( "AttackColl" ) ) ) : nullptr;
	if( attackColl )
	{ 
		attackColl->SetCollisionEnabled( IsEnabledAttackColl ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision );
		attackColl->SetVisibility( IsEnabledAttackColl );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 오브젝트 타입을 알아낸다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
EObjectType UObjectComp::GetObjectType()
{
	if( !OwningActor )
		return EObjectType::MAX;

	if( auto characterPC = Cast< ACharacterPC >( OwningActor ) )
		return EObjectType::PC;
	else if( auto characterNPC = Cast< ACharacterNPC >( OwningActor ) )
		return EObjectType::NPC;
	else if( auto staticObject = Cast< AStaticObject >( OwningActor ) )
		return EObjectType::STATIC_OBJECT;
	else if ( auto projectile = Cast< AProjectile >( OwningActor ) )
		return EObjectType::PROJECTILE;

	return EObjectType::MAX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 충돌이 시작할시에 호출되는 델리게이트에 등록하는 함수
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::HitCollBeginOverlap( UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult )
{
	if( !OwningActor || IsDie || IsFallWater ) 
		return;

	// 자기 자신 충돌은 무시한다.
	if( Cast<AActor>( OtherActor ) == OwningActor ) 
		return;

	auto boxComponent = Cast<UBoxComponent>( OtherComp );
	if( boxComponent && boxComponent->GetName().Equals( TEXT( "AttackColl" ) ) )
	{
		_ProcessHit( OtherActor );
		return;
	}

	_ProcessLandscapeHit( OtherActor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 사망 관련 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::_ProcessDie()
{
	if( !OwningActor )
		return;

	if( IsFallWater || Stat.Hp <= 0 )
	{
		IsDie = true;

		GetObjectManager().DestroyActor( OwningActor );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 피격 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::_ProcessHit( AActor* InOtherActor )
{
	auto othetObjectComp = InOtherActor ? Cast<UObjectComp>( InOtherActor->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !othetObjectComp )
		return;

	if( ( TeamType == ETeamType::MAX || othetObjectComp->GetTeamType() == ETeamType::MAX ) || othetObjectComp->GetTeamType() == TeamType )
		return;

	othetObjectComp->OnAttackSuccess();

	// 체력 감소
	float totalDamage = othetObjectComp->GetAttackCollInfo().Power * othetObjectComp->GetStat().AttackPower;
	totalDamage -= Stat.DefensePower;
	totalDamage = totalDamage > 0 ? totalDamage : 1;

	float decrease = Stat.Hp - totalDamage;
	Stat.Hp = decrease > 0 ? decrease : 0;

	FString str = OwningActor->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Stat.Hp );
	if( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );

	// 내 플레이어가 맞거나, 때린 경우에만 카메라 쉐이크
	ACharacterPC* myPlayer = GetMyGameInstance().GetMyPlayer();
	if ( !myPlayer )
		return;

	if ( OwningActor == myPlayer || InOtherActor == myPlayer )
		GetCameraManager().CameraShake( OwningActor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 초기화 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::_Init()
{
	InitStat = Stat;

	OwningActor = Cast<AActor>( GetOwner() );

	auto hitColl = OwningActor ? Cast<UBoxComponent>( OwningActor->GetDefaultSubobjectByName( TEXT( "HitColl" ) ) ) : nullptr;
	if( hitColl )
		hitColl->OnComponentBeginOverlap.AddDynamic( this, &UObjectComp::HitCollBeginOverlap );

	SetMoveSpeed( Stat.MoveSpeed );
	SetJumpPower( Stat.JumpPower );

	if( GetTeamType() == ETeamType::MAX )
	{
		if( GetObjectType() == EObjectType::PC )
			SetTeamType( ETeamType::A );
		else 
			SetTeamType( ETeamType::NEUTRAL );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 랜드스케이프 피격 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UObjectComp::_ProcessLandscapeHit( AActor* InOtherActor )
{
	// 물에 빠졌을 때
	auto waterBody = Cast<UWaterBodyComponent>( InOtherActor->GetComponentByClass( UWaterBodyComponent::StaticClass() ) );
	if( !waterBody )
		return;

	/// 가벼운 재질은 물에 빠지지 않는다.
	if( Stat.Weight <= 0.7f )
		return;

	/// 물은 물에 빠지지 않는다.
	auto charMatProperty = OwningActor ? Cast<UMaterialComp>( OwningActor->FindComponentByClass<UMaterialComp>() ) : nullptr;
	if( charMatProperty )
	{
		if( charMatProperty->GetMatState() == EMaterialState::WATER )
			return;
	}

	IsFallWater = true;
}
