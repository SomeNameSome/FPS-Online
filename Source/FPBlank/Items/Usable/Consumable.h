#pragma once
#include "CoreMinimal.h"
#include "../../Base/Item_Base.h"
#include "../../Interface/InteractableInterface.h"
#include "Consumable.generated.h"



UCLASS()
class FPBLANK_API AConsumable : public AItem_Base, public IInteractableInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly);
	UStaticMeshComponent* StaticMeshComponent;

public:
	AConsumable();
	virtual void IOnInteraction() override;
	virtual FName IGetItemName() override { return ItemName; }
};
