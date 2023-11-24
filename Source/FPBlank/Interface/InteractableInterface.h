#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"




UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};



class FPBLANK_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void IOnInteraction() = 0;
	virtual FName IGetItemName() = 0;
};
