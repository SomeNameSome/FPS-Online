#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Base.generated.h"


class AHUD_Base;

UCLASS()
class FPBLANK_API APC_Base : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SpawnPlayer();

	UFUNCTION(BlueprintCallable)
		void SetMouseSensitivity(float value);

	UFUNCTION(BlueprintPure)
		float GetMouseSensitivity() const;

	void SetupHUD();

	//void RespawnPlayer();
	//void OnPossess(APawn* InPawn) override;
};
