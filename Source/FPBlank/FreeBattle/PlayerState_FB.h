#pragma once
#include "CoreMinimal.h"
#include "../Base/PlayerState_Base.h"
#include "PlayerState_FB.generated.h"



UCLASS()
class FPBLANK_API APlayerState_FB : public APlayerState_Base
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated);
	uint8 kill;

	UPROPERTY(Replicated);
	uint8 death;

public:
	void AddKill();

	void AddDeath();

	uint8 GetKill();

	uint8 GetDeath();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
