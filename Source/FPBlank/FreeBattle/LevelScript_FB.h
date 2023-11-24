#pragma once
#include "CoreMinimal.h"
#include "../Base/LevelScript_Base.h"
#include "LevelScript_FB.generated.h"



UCLASS()
class FPBLANK_API ALevelScript_FB : public ALevelScript_Base
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	void SpawnAI();
};
