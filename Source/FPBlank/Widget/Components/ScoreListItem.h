#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreListItem.generated.h"

class UVerticalBox;
class UHorizontalBox;
class UTextBlock;

UCLASS()
class FPBLANK_API UScoreListItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget));
	UVerticalBox* _VerticalBox;

	UPROPERTY(meta = (BindWidget));
	UHorizontalBox* _HorizontalBox;

public:
	UPROPERTY(meta = (BindWidget));
	UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Kill;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Death;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Ping;

	int32 PlayerIndex;
};
