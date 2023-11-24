#include "GameMode_BR.h"
#include "../Base/HUD_Base.h"
#include <Engine/StreamableManager.h>



AGameMode_BR::AGameMode_BR()
{
	HUDClass = AHUD_Base::StaticClass();
}

void AGameMode_BR::BeginPlay()
{
	Super::BeginPlay();
}
