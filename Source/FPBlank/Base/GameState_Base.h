#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameState_Base.generated.h"



UCLASS()
class FPBLANK_API AGameState_Base : public AGameState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void SpawnDecalRPC(UMaterialInterface* DecalToSpawn, FVector InLocation, FRotator InRotation);

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void SpawnEmitterRPC(UParticleSystem* ParticleEffect, FVector InLocation);
};
