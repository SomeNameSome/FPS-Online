#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfo.generated.h"


class UCanvasPanel;
class UTextBlock;
class UProgressBar;
class UInteractionWidget;

UCLASS()
class FPBLANK_API UCharacterInfo : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(meta=(BindWidget));
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget));
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget));
	UInteractionWidget* InteractionWidget;

public:
	void SetHealthBarValue(float NewValue);

	void UpdateHealtBarValue(float UpdatingValue);

	void SetInteractionWidgetState(FName _InteractionInfo);
};
