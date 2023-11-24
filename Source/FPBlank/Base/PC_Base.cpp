#include "PC_Base.h"
#include "../Base/LevelScript_Base.h"
#include "../Base/GameMode_Base.h"
#include "Kismet/GameplayStatics.h"
#include "HUD_Base.h"
#include "GameFramework/PlayerInput.h"
#include "../Primary/CharacterPrimary.h"

DEFINE_LOG_CATEGORY_STATIC(PC_BaseLog, All, All);

void APC_Base::BeginPlay()
{
	UE_LOG(PC_BaseLog, Warning, TEXT("APC_Base::BeginPlay()"));
	Super::BeginPlay();
	if (HasAuthority())
	{
		SpawnPlayer();
	}
	if (this->IsLocalController())
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		SetupHUD();
	}
}

void APC_Base::SpawnPlayer()
{
	UE_LOG(PC_BaseLog, Warning, TEXT("APC_Base::SpawnPlayer"));
	if (GetPawn())
	{
		GetPawn()->Destroy();
	}
	ALevelScript_Base* Level = Cast<ALevelScript_Base>(GetWorld()->GetLevelScriptActor());
	if (Level)
	{
		Level->SpawnPlayer(this);
	}
	
}

void APC_Base::SetMouseSensitivity(float value)
{
	PlayerInput->SetMouseSensitivity(value);
}

float APC_Base::GetMouseSensitivity() const
{
	return PlayerInput->GetMouseSensitivity();
}

void APC_Base::SetupHUD()
{
	if (AHUD_Base* CurrentHUD = Cast<AHUD_Base>(MyHUD))
	{
		if (CurrentHUD->WidgetsAlreadyExist() == false)
		{
			CurrentHUD->CreateWidgets();
		}
	}
}