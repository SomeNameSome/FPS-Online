#pragma once
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScript_Base.generated.h"



class APlayerStart;


UCLASS()
class FPBLANK_API ALevelScript_Base : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TArray<APlayerStart*> PlayerSpawnPoints;

protected:
	virtual void BeginPlay() override;

	void SetPlayerSpawnPoints();

public:
	UFUNCTION()
	void SpawnPlayer(APlayerController* Controller);

	FTransform GetRandomPlayerSpawnPoint() const;
};
