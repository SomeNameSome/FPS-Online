#include "GameMode_FB.h"
#include "../Base/Weapon_Base.h"
#include "PC_FB.h"
#include "../AI/Character_AI.h"
#include "GameFramework/PlayerState.h"



void AGameMode_FB::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	static int8 counter = 0;
	static FString BasePlayerName = "Player";
	FString UniquePlayerName = BasePlayerName + FString::FromInt(counter);
	APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
	PS->SetPlayerName(UniquePlayerName);
	counter++;
}