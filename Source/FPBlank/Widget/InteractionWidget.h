#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UVerticalBox;
class UTextBlock;

UCLASS()
class FPBLANK_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(meta = (BindWidget));
	UVerticalBox* InteractionBox;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* InteractionButton;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* InteractionInfo;

public:
	void Add(FName _InteractionInfo);
	void Remove();
};
