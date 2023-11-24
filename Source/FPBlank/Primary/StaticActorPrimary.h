#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "../Interface/DamageableInterface.h"
#include "StaticActorPrimary.generated.h"




UCLASS()
class FPBLANK_API AStaticActorPrimary final : public AStaticMeshActor, public IDamageableInterface
{
	GENERATED_BODY()
	
public:
	void ITakeDamage(float damage, FVector InLocation, FVector InNormal) override final { ITakeDamageBlueprint(damage, InLocation, InNormal); }
	bool IIsDying() override final { return false; }
	void IAddDeath() override final {}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ITakeDamageBlueprint(float damage, FVector InLocation, FVector InNormal);
};
