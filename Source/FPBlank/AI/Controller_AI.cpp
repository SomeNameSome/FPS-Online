#include "Controller_AI.h"
#include "Perception/PawnSensingComponent.h"
#include "Character_AI.h"
#include "../FreeBattle/LevelScript_FB.h"
#include "../FreeBattle/GameMode_FB.h"
#include "../Base/Weapon_Base.h"
#include "../StaticLibrary.h"
#include "../Base/Character_Base.h"
#include "BrainComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ControllerAILog, All, All);

AController_AI::AController_AI()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	pawnInSight = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(FName("PawnSensingComponent"));
	PawnSensingComponent->bEnableSensingUpdates = false;

	FScriptDelegate delegate;
	delegate.BindUFunction(this, FName("OnSeePawn"));
	PawnSensingComponent->OnSeePawn.Add(delegate);
}

void AController_AI::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SpawnWeapon();
	}
}

void AController_AI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ACharacter_AI* AI = Cast<ACharacter_AI>(InPawn);
	if (AI)
	{
		AI->SetWeaponFromPC(AIWeapon);
	}

	ControlledAI = Cast<ACharacter_AI>(InPawn);

	RunBehaviorTree(BT_Calm);

	PawnSensingComponent->SetSensingUpdatesEnabled(true);
}

void AController_AI::OnSeePawn(APawn* _Pawn)
{
	static float interval = PawnSensingComponent->SensingInterval + 0.1f;
	GetWorldTimerManager().ClearAllTimersForObject(this);


	if (!TargetPlayer.IsValid())
	{
		TargetPlayer = Cast<ACharacter_Base>(_Pawn);
		if (!TargetPlayer.IsValid())
		{
			return;
		}
	}
	if (TargetPlayer->IIsDying())
	{
		if (pawnInSight)
		{
			OnPawnLost();
		}
	}
	else
	{
		if (!pawnInSight)
		{
			UE_LOG(ControllerAILog, Error, TEXT("RunBehaviorTree(BT_Aggressive)"));
			pawnInSight = true;
			RunBehaviorTree(BT_Aggressive);
		}
		GetWorldTimerManager().SetTimer(PawnSensingTimer, this, &AController_AI::OnPawnLost, interval, false, interval);
	}
}

void AController_AI::OnPawnLost()
{
	if (ControlledAI.IsValid())
	{
		ControlledAI->StopFire();
	}
	pawnInSight = false;
	if (TargetPlayer->IIsDying())
	{
		TargetPlayer.Reset();
	}
	RunBehaviorTree(BT_Calm);
}

void AController_AI::SpawnWeapon()
{
	AGameMode_FB* GameMode = Cast<AGameMode_FB>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		AIWeapon = GetWorld()->SpawnActor<AWeapon_Base>(GameMode->WeaponTypes[0], FTransform(), params);
	}
	else
	{
		UE_LOG(ControllerAILog, Warning, TEXT("!AController_AI GameMode"));
	}
}

void AController_AI::StopAllLogic()
{

	BrainComponent->StopLogic(FString());
}