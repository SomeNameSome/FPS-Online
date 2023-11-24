#include "LevelScript_Base.h"
#include "GameMode_Base.h"
#include "Character_Base.h"
#include "GameFramework/PlayerStart.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LevelScriptLog, All, All);

void ALevelScript_Base::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
		SetPlayerSpawnPoints();
}

void ALevelScript_Base::SpawnPlayer(APlayerController* Controller)
{
	UE_LOG(LevelScriptLog, Warning, TEXT("SpawnPlayer"));
	AGameMode_Base* GameMode = Cast<AGameMode_Base>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		FActorSpawnParameters params;
		params.Owner = Controller;
		ACharacter_Base* SpawnedCharacter = GetWorld()->SpawnActor<ACharacter_Base>(GameMode->CharacterClass, GetRandomPlayerSpawnPoint(), params);
		Controller->Possess(SpawnedCharacter);
	}
}

FTransform ALevelScript_Base::GetRandomPlayerSpawnPoint() const
{
	int32 ArraySize = PlayerSpawnPoints.Num();
	if (ArraySize > 0)
	{
		return PlayerSpawnPoints[FMath::RandRange(0, --ArraySize)]->GetTransform();
	}
	else
	{
		UE_LOG(LevelScriptLog, Error, TEXT("PlayerSpawnPoints.Num() == 0"));
	}
	return FTransform();
}

void ALevelScript_Base::SetPlayerSpawnPoints()
{
	for (TActorIterator<APlayerStart> Iter(GetWorld()); Iter; ++Iter)
	{
		PlayerSpawnPoints.Add(*Iter);
	}
}