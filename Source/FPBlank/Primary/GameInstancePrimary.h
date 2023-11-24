#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstancePrimary.generated.h"



UCLASS()
class FPBLANK_API UGameInstancePrimary final : public UGameInstance
{
	GENERATED_BODY()
	
private:
	bool enableAI;

public:
	UFUNCTION(BlueprintCallable)
	void SetEnableAI(bool enable);

	bool AIIsEnabled();
};
