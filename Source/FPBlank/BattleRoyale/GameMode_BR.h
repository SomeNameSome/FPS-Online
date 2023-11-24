#pragma once
#include "CoreMinimal.h"
#include "../Base/GameMode_Base.h"
#include "GameMode_BR.generated.h"


class ACharacter_BR;

UCLASS()
class FPBLANK_API AGameMode_BR : public AGameMode_Base
{
	GENERATED_BODY()

public:
	AGameMode_BR();

protected:
	void BeginPlay() override;

};
