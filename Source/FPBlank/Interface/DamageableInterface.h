#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"



UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};


class FPBLANK_API IDamageableInterface
{
	GENERATED_BODY()


public:
	virtual void ITakeDamage(float damage, FVector InLocation, FVector InNormal) = 0;
	virtual bool IIsDying() = 0;
	virtual void IAddDeath() = 0;
};
