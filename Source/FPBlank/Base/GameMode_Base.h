#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_Base.generated.h"

class ACharacter_Base;
class AWeapon_Base;

UCLASS()
class FPBLANK_API AGameMode_Base : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<ACharacter_Base> CharacterClass;

	UPROPERTY(EditDefaultsOnly);
	TArray<TSubclassOf<AWeapon_Base>> WeaponTypes;
	
public:
	AGameMode_Base();
};
