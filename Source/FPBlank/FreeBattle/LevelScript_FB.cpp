#include "LevelScript_FB.h"
#include "GameMode_FB.h"
#include "../Primary/GameInstancePrimary.h"


void ALevelScript_FB::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		UGameInstancePrimary* GI = GetGameInstance<UGameInstancePrimary>();
		if (GI)
		{
			GI->AIIsEnabled() ? SpawnAI() : false;
		}
	}
}

void ALevelScript_FB::SpawnAI()
{
	AGameMode_FB* GameMode = Cast<AGameMode_FB>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		APawn* NewAI = GetWorld()->SpawnActor<APawn>(GameMode->CharacterAIClass, GetRandomPlayerSpawnPoint());
		if (NewAI)
		{
			NewAI->SpawnDefaultController();
			NewAI->SetOwner(NewAI->GetController());
		}
	}
}