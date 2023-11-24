#pragma once
#include "CoreMinimal.h"
#include "../Base/GameMode_Base.h"
#include "../AI/Character_AI.h"
#include "GameMode_FB.generated.h"

class AWeapon_Base;
class ACharacter_AI;

UCLASS()
class FPBLANK_API AGameMode_FB : public AGameMode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<ACharacter_AI> CharacterAIClass;

public:
	void PostLogin(APlayerController* NewPlayer) override;

};
