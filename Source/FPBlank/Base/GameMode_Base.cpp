#include "GameMode_Base.h"
#include "Character_Base.h"
#include "../Base/Weapon_Base.h"
#include "HUD_Base.h"


AGameMode_Base::AGameMode_Base()
{
	HUDClass = AHUD_Base::StaticClass();
}