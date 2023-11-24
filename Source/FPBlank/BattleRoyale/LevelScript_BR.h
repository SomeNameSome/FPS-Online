#pragma once
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "../Base/LevelScript_Base.h"
#include "LevelScript_BR.generated.h"


class ATargetPoint;
class AWeapon_Base;


UCLASS()
class FPBLANK_API ALevelScript_BR : public ALevelScript_Base
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TArray<AWeapon_Base*> WeaponArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TArray<ATargetPoint*> WeaponSpawnPoints;

public:
	ALevelScript_BR();
	void SetWeaponSpawnPoints();
	FTransform GetRandomWeaponSpawnPoint() const;
	const TArray<AWeapon_Base*>& GetWeaponArray() const;

protected:
	void BeginPlay() override;

public:
	UFUNCTION()
	void SpawnWeapon();
};
