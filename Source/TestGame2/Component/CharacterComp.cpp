// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterComp.h"
#include "MaterialComp.h"
#include "WeaponComp.h"
#include "FloatingBarComp.h"
#include "../ETC/SDB.h"
#include "../Actor/CharacterPC.h"
#include "../Actor/CharacterNPC.h"
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
#include "Components/WidgetComponent.h"
#include "WaterBodyComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UCharacterComp::UCharacterComp()
:
OwningCharacter ( nullptr ),
AnimState       ( EAnimState::IDLE_RUN ),
CurSkillInfo    ( nullptr ),
IsForceMove     ( false ),
LandOnce        ( false ),
FallWaterTime   ( 0 ),
MontagePlayTime ( 0 ),
DeathTime       ( 0 )
{
	PrimaryComponentTick.bCanEverTick = true;
}

UCharacterComp::~UCharacterComp()
{
	CoolingSkills.Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::BeginPlay()
{
	Super::BeginPlay();

	_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Tick
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	_AnimStateChange();
	_ProcessHold( DeltaTime );
	_ProcessMove();
	_ProcessAccTime( DeltaTime );
	_ProcessMp( DeltaTime );
	_CoolingSkills( DeltaTime );
	_FallingWater( DeltaTime );
	_ProcessLand();

	ResetInfo();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 정보를 초기화한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::ResetInfo( bool InForceReset )
{
	if( InForceReset ||
		AnimState == EAnimState::IDLE_RUN ||
		AnimState == EAnimState::JUMP ||
		AnimState == EAnimState::DIE )
	{
		SetIsEnabledAttackColl( false );
		SetAttackCollInfo( FCollisionInfo() );
		IsEnableDerivedKey = false;
		IsForceMove = false;
		CurSkillInfo = nullptr;
		MontagePlayTime = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 몽타주를 플레이한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::MontagePlay( const FString& InMontagePath, float InScale )
{
	UAnimMontage* anim = LoadObject<UAnimMontage>( NULL, *InMontagePath, NULL, LOAD_None, NULL );
	if ( !anim )
		return;

	MontagePlay( anim, InScale );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 몽타주를 플레이한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::MontagePlay( UAnimMontage* InMontage, float InScale )
{
	if( !OwningCharacter )
		return;

	if ( !OwningCharacter->GetMesh() )
		return;

	UAnimInstance* animInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if( !animInstance || !InMontage )
		return;

	ResetInfo( true );

	animInstance->Montage_Play( InMontage, InScale );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬을 플레이한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::SkillPlay( int InSkillNum )
{
	const auto& skillInfo = GetDataInfoManager().GetSkillInfos().Find( InSkillNum );
	if ( !skillInfo )
		return false;

	// 파생스킬을 발동 시킬 것인지 확인
	if ( skillInfo->DerivedSkillNum != 0 && IsEnableDerivedKey )
		return SkillPlay( skillInfo->DerivedSkillNum );

	// Mp 확인
	if( skillInfo->CostMP > Stat.Mp )
		return false;

	// 쿨타임 확인
	if ( IsCoolingSkill( skillInfo->Num ) )
		return false;

	// 현재 스킬 사용 가능한 AnimState / SkillNum 인지 확인
	bool isEmptyEnableState = skillInfo->PlayEnableState.IsEmpty();
	bool isEmptyEnableSkillNum = skillInfo->PlayEnableSkillNum.IsEmpty();
	bool isFindEnableState = skillInfo->PlayEnableState.Find( AnimState ) != INDEX_NONE;
	bool isFindEnableSkillNum = skillInfo->PlayEnableSkillNum.Find( CurSkillInfo ? CurSkillInfo->Num : 0 ) != INDEX_NONE;

	// 사용가능한 AnimState / SkillNum 이 둘 다 설정 되어 있을 경우 둘 중 하나만 만족하면 된다.
	if ( !isEmptyEnableState && !isEmptyEnableSkillNum )
	{
		if ( !isFindEnableState && !isFindEnableSkillNum )
			return false;
	}
	else
	{   // 사용가능한 AnimState / SkillNum 둘 중 하나만 설정 되어 있을 경우 설정 되어 있는 것만 만족하면 된다.
		if ( !isEmptyEnableState && !isFindEnableState )
			return false;
		if ( !isEmptyEnableSkillNum && !isFindEnableSkillNum )
			return false;
	}

	if ( skillInfo->PlaySpeedType == ESkillPlaySpeedType::DEFAULT )
		MontagePlay( skillInfo->AnimPath );
	else
		MontagePlay( skillInfo->AnimPath, skillInfo->PlaySpeedType == ESkillPlaySpeedType::ATTACK_SPEED ? Stat.AttackSpeed : Stat.MoveSpeed );

	_RegisterCoolTime( *skillInfo );

	CurSkillInfo = skillInfo;

	Stat.Mp = FMath::Clamp( Stat.Mp - skillInfo->CostMP, 0, Stat.Mpm );

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 공격 성공 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::OnAttackSuccess()
{
	auto WeaponComp = OwningActor ? Cast<UWeaponComp>( OwningActor->FindComponentByClass<UWeaponComp>() ) : nullptr;
	if( !WeaponComp )
		return;

	WeaponComp->SubWeaponDurability();

	// 역경직 시간 추가
	HoldTime += Const::HOLD_TIME_INCREASE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동속도를 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::SetMoveSpeed( float InMoveSpeed )
{
	Stat.MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = Stat.MoveSpeed * Const::DEFAULT_MOVE_SPEED;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 점프력을 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::SetJumpPower( float InJumpPower )
{
	Stat.JumpPower = InJumpPower;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->JumpZVelocity = Stat.JumpPower * Const::DEFAULT_JUMP_POWER;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동할 위치를 셋팅한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::SetMovePos( float InMovePower, bool InIsKnockBack )
{
	if( !OwningCharacter )
		return;

	const FRotator rotation = OwningCharacter->GetActorRotation();
	const FRotator yawRotation( 0, rotation.Yaw, 0 );
	const FVector  direction = FRotationMatrix( yawRotation ).GetUnitAxis( EAxis::X );

	if( InIsKnockBack )
		MovePos = OwningCharacter->GetActorLocation() - ( direction * InMovePower );
	else
		MovePos = OwningCharacter->GetActorLocation() + ( direction * ( InMovePower * Stat.MoveSpeed ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 현재 몽타주 이름을 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
FString UCharacterComp::GetCurMontageName()
{
	auto curMontage = OwningCharacter ? OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() : nullptr;
	FString curMontageName = curMontage ? curMontage->GetName() : "";

	return curMontageName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 해당 스킬이 쿨타임이 돌고 있는지 여부를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::IsCoolingSkill( int InSkillNum )
{
	float* cooltime = CoolingSkills.Find( InSkillNum );
	if ( cooltime )
	{
		if ( *cooltime > 0 )
			return true;
		else
			return false;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 몽타주 재생 초기시간인지 여부를 반환한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::IsMontageInitialTime()
{
	if( AnimState != EAnimState::COMMON_ACTION )
		return false;

	if( MontagePlayTime > Const::MONTAGE_INITIAL_TIME )
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 역경직 상태 여부를 반환한다. 
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::IsHold()
{
	return HoldTime > 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 사망 관련 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessDie()
{
	if( !OwningCharacter )
		return;

	if( !IsDie && AnimState == EAnimState::DIE )
	{
		IsDie = true;

		switch( GetObjectType() )
		{
		case  EObjectType::NPC:
		{
			AMonsterAIController* characterNPCController = Cast< AMonsterAIController >( OwningCharacter->GetController() );
			if( characterNPCController )
				characterNPCController->StopAI();

			OwningCharacter->GetMesh()->SetSimulatePhysics( true );
			OwningCharacter->GetMesh()->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
			GetObjectManager().SpawnParticle( TEXT( "Die" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
		}
		break;
		case  EObjectType::PC:
		{
			OwningCharacter->GetMesh()->SetSimulatePhysics( true );
			OwningCharacter->GetMesh()->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
			GetObjectManager().SpawnParticle( TEXT( "Die" ), OwningCharacter, OwningCharacter->GetActorLocation(), OwningCharacter->GetActorRotation() );
			/*if( UMyAnimInstance* animInstance = Cast<UMyAnimInstance>( OwningCharacter->GetMesh()->GetAnimInstance() ) )
			{
				auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
				animInstance->Montage_Stop( 0.f, curMontage );
				animInstance->IsDie = true;
			}*/
		}
		break;
		case  EObjectType::STATIC_OBJECT:
		{
			GetObjectManager().DestroyActor( OwningCharacter );
		}
		break;
		}

		if( UMyAnimInstance* animInstance = Cast<UMyAnimInstance>( OwningCharacter->GetMesh()->GetAnimInstance() ) )
		{
			auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
			animInstance->Montage_Stop( 0.f, curMontage );
		}
		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( TEXT( "NoCollision" ) );
		_UpdateHpBar();
	}

	if( AnimState == EAnimState::DIE && DeathTime >= Const::DEAD_ACTOR_DESTROY_TIME )
	{
		GetObjectManager().DestroyActor( OwningCharacter );

		if ( GetObjectType() == EObjectType::PC )
		{
			FString levelName = L"/Game/Maps/Stage1/Stage1";
			UGameplayStatics::OpenLevel( this, *levelName );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 피격 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessHit( AActor* InOtherActor )
{
	auto othetObjectComp = InOtherActor ? Cast<UObjectComp>( InOtherActor->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !othetObjectComp )
		return;

	auto othetMatComp = InOtherActor ? Cast<UMaterialComp>( InOtherActor->FindComponentByClass<UMaterialComp>() ) : nullptr;
	auto myMatComp    = OwningActor  ? Cast<UMaterialComp>( OwningActor->FindComponentByClass<UMaterialComp>() ) : nullptr;

	if ( othetObjectComp->GetTeamType() == ETeamType::MAX || TeamType == ETeamType::MAX )
		return;

	if ( othetObjectComp->GetTeamType() == TeamType && !othetObjectComp->Stat.IsTyrant )
		return;

	othetObjectComp->OnAttackSuccess();

	// 체력 감소
	float totalDamage = othetObjectComp->GetAttackCollInfo().Power * othetObjectComp->GetStat().AttackPower;
	totalDamage -= Stat.DefensePower;
	totalDamage = totalDamage > 0 ? totalDamage : 1;

	float decrease = Stat.Hp - totalDamage;
	Stat.Hp = decrease > 0 ? decrease : 0;

	float myIntensity    = myMatComp    ? myMatComp->GetIntensity()    : 1.f;
	float otherIntensity = othetMatComp ? othetMatComp->GetIntensity() : 1.f;

	// 경직
	if( otherIntensity >= myIntensity )
	{
		float addTimeToDamage = totalDamage / Stat.Hpm;
		float subTimeToMyStrength = Stat.Strength * 0.01f;
		MontagePlay( HitAnim, 1.0f - addTimeToDamage + subTimeToMyStrength );
		LookAt( Cast<ACharacter>( InOtherActor ) );

		// 넉백
		float knockbackPower = othetObjectComp->GetAttackCollInfo().KnockBackPower - ( Stat.Strength * 1.5f );
		if( knockbackPower > 0 )
		{
			SetMovePos( knockbackPower, true );
			SetIsForceMove( true );
		}

		// 역경직 시간 추가
		HoldTime += Const::HOLD_TIME_INCREASE_VALUE;
	}

	_ProcessCameraShake( InOtherActor );

	_UpdateHpBar();

	//FString str = OwningActor->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Stat.Hp );
	//if ( GEngine )
	//	GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 초기화 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_Init()
{
	OwningCharacter = Cast<ACharacter>( OwningActor );

	ResetInfo( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 애니메이션 상태를 교체한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_AnimStateChange()
{
	if( Stat.Hp <= 0 )
	{
		AnimState = EAnimState::DIE;
		return;
	}

	if( !OwningCharacter )
		return;

	UMyAnimInstance* animInstance = Cast<UMyAnimInstance>( OwningCharacter->GetMesh()->GetAnimInstance() );
	if( !animInstance )
		return;

	if( auto curMontage = animInstance->GetCurrentActiveMontage() )
	{
		AnimState = EAnimState::COMMON_ACTION;
	}
	else
	{
		if( auto moveComponent = OwningCharacter->GetMovementComponent(); moveComponent )
			moveComponent->IsFalling() ? AnimState = EAnimState::JUMP : AnimState = EAnimState::IDLE_RUN;
	}

	animInstance->AnimState = AnimState;
	animInstance->AnimSubState = CurSkillInfo ? CurSkillInfo->AnimSubState : EAnimSubState::DEFAULT;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 이동 관련 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessMove()
{
	if ( IsHold() )
		return;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

	if( !characterMovement )
		return;

	if( AnimState == EAnimState::IDLE_RUN || AnimState == EAnimState::JUMP )
	{
		characterMovement->MaxWalkSpeed = Stat.MoveSpeed * Const::DEFAULT_MOVE_SPEED;
	}
	else
	{
		if( IsForceMove )
		{
			float dest_X = FMath::Lerp( characterMovement->GetActorLocation().X, MovePos.X, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			float dest_Y = FMath::Lerp( characterMovement->GetActorLocation().Y, MovePos.Y, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			float dest_Z = FMath::Lerp( characterMovement->GetActorLocation().Z, MovePos.Z, GetWorld()->GetDeltaSeconds() * Const::ANIM_LERP_MULITPLIER );
			FVector dest = FVector( dest_X, dest_Y, dest_Z );

			OwningCharacter->SetActorLocation( dest, true );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬 쿨타임을 등록한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_RegisterCoolTime( const FSkillInfo& InSkillInfo )
{
	if( InSkillInfo.CoolTime <= 0 )
		return;

	if( float* coolTime = CoolingSkills.Find( InSkillInfo.Num ) )
		*coolTime = InSkillInfo.CoolTime;
	else
		CoolingSkills.Add( InSkillInfo.Num, InSkillInfo.CoolTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 체력바를 업데이트 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_UpdateHpBar()
{
	auto floatingBarComp = Cast<UFloatingBarComp>( OwningCharacter->GetDefaultSubobjectByName( TEXT( "FloatingBarComp" ) ) );
	if ( !floatingBarComp )
		return;

	floatingBarComp->UpdateHpBar( Stat.Hp / Stat.Hpm );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 스킬 쿨타임을 돌린다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_CoolingSkills( float InDeltaTime )
{
	for( auto& [_, remainCoolTime] : CoolingSkills )
	{
		if( remainCoolTime > 0 )
			remainCoolTime -= InDeltaTime;
		else
			continue;

		if( remainCoolTime < 0 )
			remainCoolTime = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 물에 빠지는 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_FallingWater( float InDeltaTime )
{
	if( !IsFallWater || Stat.Hp <= 0 )
		return;

	auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	bool isNotHitAnim = curMontage && HitAnim && curMontage->GetName() != HitAnim->GetName();
	if( !curMontage || isNotHitAnim )
		MontagePlay( HitAnim, Const::WATER_FALL_ANIM_TIME );

	if( FallWaterTime <= 3.f )
	{
		FallWaterTime += InDeltaTime;
		FVector originPos = OwningCharacter->GetMesh()->GetRelativeLocation();
		OwningCharacter->GetMesh()->SetRelativeLocation( FVector( originPos.X, originPos.Y, originPos.Z - 1.7f ) );
	}
	else
	{
		Stat.Hp = 0;
		IsFallWater = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 착지 로직을 실행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessLand()
{
	auto curMontage = OwningCharacter ? OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() : nullptr;

	auto moveComponent = OwningCharacter->GetMovementComponent();
	if( moveComponent )
	{
		if( moveComponent->IsFalling() )
		{
			LandOnce = true;
		}
		else if( LandOnce )
		{
			if( !curMontage )
				MontagePlay( LandAnim );
			
			GetCameraManager().CameraShake( OwningCharacter, 1.f, true );
			LandOnce = false;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 역경직 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessHold( float InDeltaTime )
{
	if ( !IsHold() )
		return;

	if ( !OwningCharacter )
		return;

	auto animInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if ( !animInstance )
		return;

	auto curMontage = animInstance->GetCurrentActiveMontage();
	if ( !curMontage )
		return;

	HoldTime -= InDeltaTime;

	if ( HoldTime <= 0 )
	{
		HoldTime = 0;
		animInstance->Montage_Resume( curMontage );
		return;
	}

	animInstance->Montage_Pause( curMontage );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief Mp 로직을 수행한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessMp( float InDeltaTime )
{
	if( AnimState == EAnimState::IDLE_RUN )
		Stat.Mp = FMath::Clamp( Stat.Mp + ( InDeltaTime * Const::MP_RECOVERY_VALUE ), 0, Stat.Mpm );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief 누적 시간 관련 로직 처리를 한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessAccTime( float InDeltaTime )
{
	if ( AnimState == EAnimState::COMMON_ACTION )
	{
		MontagePlayTime += InDeltaTime;
	}
	else if ( AnimState == EAnimState::DIE )
	{
		DeathTime += InDeltaTime;
	}
}