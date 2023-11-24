#include "GameState_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"



void AGameState_Base::SpawnDecalRPC_Implementation(UMaterialInterface* DecalToSpawn, FVector InLocation, FRotator InRotation)
{
	float fadeTime = 10.f;
	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalToSpawn, FVector(5.f, 5.f, 5.f), InLocation, InRotation, fadeTime);
	if (Decal)
		Decal->SetFadeScreenSize(0.f);
}

void AGameState_Base::SpawnEmitterRPC_Implementation(UParticleSystem* ParticleEffect, FVector InLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, InLocation);
}