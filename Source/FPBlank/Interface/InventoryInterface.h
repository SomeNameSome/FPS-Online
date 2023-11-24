#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"


class AWeapon_Base;
class UInventoryWidget;


UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};


class FPBLANK_API IInventoryInterface
{
	GENERATED_BODY()

public:
	virtual void IOnAddWeapon(AWeapon_Base* NewWeapon) = 0;
	virtual UInventoryWidget* IGetIventoryWidget() = 0;
	virtual bool IIsLocallyControlled() const = 0;
	virtual bool ICurrentWeaponInArm() = 0;
	virtual void IOnRemoveWeapon(AWeapon_Base* WeaponToRemove) = 0;
};
