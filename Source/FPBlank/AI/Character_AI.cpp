#include "Character_AI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Base/Weapon_Base.h"
#include "Controller_AI.h"
#include "FPBlank/StaticLibrary.h"
#include "../FreeBattle/LevelScript_FB.h"

DEFINE_LOG_CATEGORY_STATIC(CharacterAILog, All, All);

ACharacter_AI::ACharacter_AI()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CharacterState = 0;
	CharacterState |= static_cast<uint8>(ECharacterState::BattleMode);

	SafeMode = CreateDefaultSubobject<USceneComponent>(TEXT("SafeModeRifle"));
	SafeMode->SetupAttachment(GetMesh(), "SafeModeRifle");
	SafeMode->SetRelativeLocationAndRotation(FVector(-3.5f, 11.0f, -1.5f), FRotator(2.0f, -60.0f, -85.0f));

	BattleMode = CreateDefaultSubobject<USceneComponent>(TEXT("BattleModeRifle"));
	BattleMode->SetupAttachment(GetMesh(), "BattleModeRifle");
	BattleMode->SetRelativeLocationAndRotation(FVector(-7.9f, 4.8f, 0.0f), FRotator(0.0f, 90.0f, 10.0f));

	AnimChangeBattleMode = CreateDefaultSubobject<UAnimMontage>(TEXT("ChangeBattleMode"));
}

void ACharacter_AI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacter_AI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	ACharacter::GetLifetimeReplicatedProps(OutLifetimeProps);

	DISABLE_REPLICATED_PROPERTY(ACharacter_AI, Health);
	DOREPLIFETIME(ACharacter_AI, CharacterState);
	DOREPLIFETIME_CONDITION(ACharacter_AI, ControlRotation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ACharacter_AI, CurrentWeapon, COND_InitialOnly);
}

void ACharacter_AI::BeginPlay()
{
	ACharacter::BeginPlay();
}

void ACharacter_AI::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
}

void ACharacter_AI::Restart()
{
	ACharacter::Restart();
}

void ACharacter_AI::ITakeDamage(float damage, FVector InLocation, FVector InNormal)
{
	Health += damage;
	if (Health <= 0.f)
	{
		DestroyCharacter();
	}
}

void ACharacter_AI::DestroyCharacter()
{
	Super::DestroyCharacter();
	StopFire();
	AController_AI* AIC = Cast<AController_AI>(Controller);
	if (AIC)
	{
		AIC->StopAllLogic();
	}
	ALevelScript_FB* Level = Cast<ALevelScript_FB>(GetWorld()->GetLevelScriptActor());
	if (Level)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		GetWorldTimerManager().SetTimer(Timer, [this, Level]()
		{
			CurrentWeapon->Destroy();
			Level->SpawnAI();
			Destroy();
			
		}, 5.f, false, 5.f);
	}
}

void ACharacter_AI::Reloading()
{
	AddState(ECharacterState::PlayingAnim);
	//CurrentWeapon->MulticastCallback("Reloading");
}

void ACharacter_AI::Fire()
{
	if (!GetWorldTimerManager().IsTimerActive(Timer))
	{
		StopRun();
		AddState(ECharacterState::Firing);
		//GetWorldTimerManager().SetTimer(Timer, CurrentWeapon.Get(), &AWeapon_Base::FireAI, CurrentWeapon->GetFireRateDelay(), true, 0.f);
	}
	else
	{
		UE_LOG(CharacterAILog, Warning, TEXT("Can`t fire"));
	}
}

void ACharacter_AI::StopFire()
{
	if (State(ECharacterState::Firing))
		RemoveState(ECharacterState::Firing);
}

void ACharacter_AI::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	AddState(ECharacterState::Runing);
}

void ACharacter_AI::StopRun()
{
	if (State(ECharacterState::Runing))
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		RemoveState(ECharacterState::Runing);
	}
}

void ACharacter_AI::ChangeBattleMode()
{
	UStaticLibrary::PlayMontage(GetMesh(), AnimChangeBattleMode, this, "ChangeBattleModeNotifyBegin");
}

void ACharacter_AI::SetWeaponFromPC(AWeapon_Base* Weapon)
{
	if (Weapon)
	{
		CurrentWeapon = Weapon;
		OnRep_Weapon();
	}
	else
	{
		UE_LOG(CharacterAILog, Error, TEXT("!Weapon SetWeaponFromPC"));
	}
}

void ACharacter_AI::OnRep_Weapon()
{
	CurrentWeapon->SetCollisionState(false);
	CurrentWeapon->SetHolder(this);
	AttachWeaponToActor();
}

void ACharacter_AI::AttachWeaponToActor()
{
	if (CurrentWeapon.IsValid())
	{
		FAttachmentTransformRules attachRifleRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		CurrentWeapon->AttachToComponent(BattleMode, attachRifleRules, FName("BattleModeRifle"));
	}
	else
	{
		UE_LOG(CharacterAILog, Error, TEXT("!CurrentWeapon AttachWeaponToActor"));
	}
}