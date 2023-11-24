#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreList.generated.h"

class UCanvasPanel;
class UTextBlock;
class UBorder;
class UVerticalBox;
class UScrollBox;
class UHorizontalBox;
class UScoreListItem;

UCLASS()
class FPBLANK_API UScoreList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget));
	UCanvasPanel* _CanvasPanel;

	UPROPERTY(meta = (BindWidget));
	UBorder* _Border;

	UPROPERTY(meta = (BindWidget));
	UVerticalBox* _VerticalBox;

	UPROPERTY(meta = (BindWidget));
	UHorizontalBox* _HorizontalBox;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget));
	UScrollBox* ScrollBoxList;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Title_Name;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Title_Kill;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Title_Death;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Title_Ping;

	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<UScoreListItem> ItemClass;

	UPROPERTY(EditDefaultsOnly);
	TArray<UScoreListItem*> List;

public:
	void AddItem(FString _PlayerName, int32 ping);

	void AddItem(FString _PlayerName, uint8 kill, uint8 death, uint8 ping);

	void UpdateItem(FString PlayerNameToUpdate, int32 kill = 0, int32 death = 0);

	void ClearItemList();

	void AddKill(FString _PlayerName);

	void AddDeath(FString _PlayerName);

	void UpdatePing(FString _PlayerName, int32 ping);

	int32 ConvertToInt(FText From);

	FText ConvertToText(int32 From);

private:
	UScoreListItem* GetItemByPlayerName(FString _PlayerName);
};
