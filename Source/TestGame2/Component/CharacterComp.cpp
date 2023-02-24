// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterComp.h"
#include "MaterialComp.h"
#include "WeaponComp.h"
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
#include "WaterBodyComponent.h"
#include "LandscapeComponent.h"
#include "LandscapeProxy.h"
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
	_CoolingSkills( DeltaTime );
	_FallingWater( DeltaTime );
	_ProcessLand();

	ResetInfo();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ������ �ʱ�ȭ�Ѵ�.
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
//// @brief ��Ÿ�ָ� �÷����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::MontagePlay( const FString& InMontagePath, float InScale )
{
	UAnimMontage* anim = LoadObject<UAnimMontage>( NULL, *InMontagePath, NULL, LOAD_None, NULL );
	if ( !anim )
		return;

	MontagePlay( anim, InScale );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ��Ÿ�ָ� �÷����Ѵ�.
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
//// @brief ��ų�� �÷����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::SkillPlay( int InSkillNum )
{
	const auto& skillInfo = GetDataInfoManager().GetSkillInfos().Find( InSkillNum );
	if ( !skillInfo )
		return false;

	// �Ļ���ų�� �ߵ� ��ų ������ Ȯ��
	if ( skillInfo->DerivedSkillNum != 0 && IsEnableDerivedKey )
		return SkillPlay( skillInfo->DerivedSkillNum );

	// ��Ÿ�� Ȯ��
	if ( IsCoolingSkill( skillInfo->Num ) )
		return false;

	// ���� ��ų ��� ������ AnimState / SkillNum ���� Ȯ��
	bool isEmptyEnableState = skillInfo->PlayEnableState.IsEmpty();
	bool isEmptyEnableSkillNum = skillInfo->PlayEnableSkillNum.IsEmpty();
	bool isFindEnableState = skillInfo->PlayEnableState.Find( AnimState ) != INDEX_NONE;
	bool isFindEnableSkillNum = skillInfo->PlayEnableSkillNum.Find( CurSkillInfo ? CurSkillInfo->Num : 0 ) != INDEX_NONE;

	// ��밡���� AnimState / SkillNum �� �� �� ���� �Ǿ� ���� ��� �� �� �ϳ��� �����ϸ� �ȴ�.
	if ( !isEmptyEnableState && !isEmptyEnableSkillNum )
	{
		if ( !isFindEnableState && !isFindEnableSkillNum )
			return false;
	}
	else
	{   // ��밡���� AnimState / SkillNum �� �� �ϳ��� ���� �Ǿ� ���� ��� ���� �Ǿ� �ִ� �͸� �����ϸ� �ȴ�.
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

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ���� ���� ó���� �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::OnAttackSuccess()
{
	auto WeaponComp = OwningActor ? Cast<UWeaponComp>( OwningActor->FindComponentByClass<UWeaponComp>() ) : nullptr;
	if( !WeaponComp )
		return;

	WeaponComp->SubWeaponDurability();

	// ������ �ð� �߰�
	HoldTime += Const::HOLD_TIME_INCREASE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �̵��ӵ��� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::SetMoveSpeed( float InMoveSpeed )
{
	Stat.MoveSpeed = InMoveSpeed;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->MaxWalkSpeed = Stat.MoveSpeed * Const::DEFAULT_MOVE_SPEED;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �������� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::SetJumpPower( float InJumpPower )
{
	Stat.JumpPower = InJumpPower;

	auto characterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	if( characterMovement )
		characterMovement->JumpZVelocity = Stat.JumpPower * Const::DEFAULT_JUMP_POWER;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �̵��� ��ġ�� �����Ѵ�.
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
//// @brief ���� ��Ÿ�� �̸��� ��ȯ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
FString UCharacterComp::GetCurMontageName()
{
	auto curMontage = OwningCharacter ? OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() : nullptr;
	FString curMontageName = curMontage ? curMontage->GetName() : "";

	return curMontageName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ش� ��ų�� ��Ÿ���� ���� �ִ��� ���θ� ��ȯ�Ѵ�.
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
//// @brief ��Ÿ�� ��� �ʱ�ð����� ���θ� ��ȯ�Ѵ�.
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
//// @brief ������ ���� ���θ� ��ȯ�Ѵ�. 
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool UCharacterComp::IsHold()
{
	return HoldTime > 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief ��� ���� ������ �����Ѵ�.
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

		OwningCharacter->GetCapsuleComponent()->SetCollisionProfileName( TEXT( "NoCollision" ) );
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
//// @brief �ǰ� ó���� �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_ProcessHit( AActor* InOtherActor )
{
	auto othetObjectComp = InOtherActor ? Cast<UObjectComp>( InOtherActor->FindComponentByClass<UObjectComp>() ) : nullptr;
	if( !othetObjectComp )
		return;

	if ( othetObjectComp->GetTeamType() == ETeamType::MAX || TeamType == ETeamType::MAX )
		return;

	if ( othetObjectComp->GetTeamType() == TeamType && !othetObjectComp->Stat.IsTyrant )
		return;

	othetObjectComp->OnAttackSuccess();

	// ü�� ����
	float totalDamage = othetObjectComp->GetAttackCollInfo().Power * othetObjectComp->GetStat().AttackPower;
	totalDamage -= Stat.DefensePower;
	totalDamage = totalDamage > 0 ? totalDamage : 1;

	float decrease = Stat.Hp - totalDamage;
	Stat.Hp = decrease > 0 ? decrease : 0;

	// ����
	if( Stat.Hpm * ( 0.1f + ( Stat.Strength * 0.01f ) ) < totalDamage )
	{
		MontagePlay( HitAnim, 1.0f + ( Stat.Strength * 0.01f ) );
		LookAt( Cast<ACharacter>( InOtherActor ) );

		// �˹�
		float knockbackPower = othetObjectComp->GetAttackCollInfo().KnockBackPower - ( Stat.Strength * 1.5f );
		if( knockbackPower > 0 )
		{
			SetMovePos( knockbackPower, true );
			SetIsForceMove( true );
		}

		// ������ �ð� �߰�
		HoldTime += Const::HOLD_TIME_INCREASE_VALUE;
	}

	_ProcessCameraShake( InOtherActor );

	FString str = OwningActor->GetName() + TEXT( " : HitColl -> HP : " ) + FString::FromInt( (int)Stat.Hp );
	if ( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, str );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ʱ�ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_Init()
{
	OwningCharacter = Cast<ACharacter>( OwningActor );

	ResetInfo( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//// @brief �ִϸ��̼� ���¸� ��ü�Ѵ�.
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
//// @brief �̵� ���� ������ �����Ѵ�.
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
//// @brief ��ų ��Ÿ���� ����Ѵ�.
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
//// @brief ��ų ��Ÿ���� ������.
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
//// @brief ���� ������ ó���� �Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UCharacterComp::_FallingWater( float InDeltaTime )
{
	if( !IsFallWater || Stat.Hp <= 0 )
		return;

	auto curMontage = OwningCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	bool isNotHitAnim = curMontage && HitAnim && curMontage->GetName() != HitAnim->GetName();
	if( !curMontage || isNotHitAnim )
		MontagePlay( HitAnim, 0.3f );

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
//// @brief ���� ������ �����Ѵ�.
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
//// @brief ������ ������ �����Ѵ�.
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
//// @brief ���� �ð� ���� ���� ó���� �Ѵ�.
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