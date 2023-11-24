#pragma once
#include "CoreMinimal.h"
#include "../Base/PC_Base.h"
#include "PC_FB.generated.h"


class AWeapon_Base;

UCLASS()
class FPBLANK_API APC_FB : public APC_Base
{
	GENERATED_BODY()
	
protected:
	UPROPERTY();
	TArray<AWeapon_Base*> CharacterWeapons;

protected:
	void BeginPlay() override;

public:
	void RespawnPlayer();
	void SpawnWeapon();
	void AttachWeaponToCharacter();

private:
	UFUNCTION(Client, Reliable)
	void ResetHUD();
};
