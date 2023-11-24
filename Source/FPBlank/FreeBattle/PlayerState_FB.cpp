#include "PlayerState_FB.h"
#include <Net/UnrealNetwork.h>



void APlayerState_FB::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerState_FB, kill);
	DOREPLIFETIME(APlayerState_FB, death);
}

void APlayerState_FB::AddKill()
{
	kill++;
}

void APlayerState_FB::AddDeath()
{
	death++;
}

uint8 APlayerState_FB::GetKill()
{
	return kill;
}

uint8 APlayerState_FB::GetDeath()
{
	return death;
}