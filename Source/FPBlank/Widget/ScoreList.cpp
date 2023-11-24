#include "ScoreList.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScoreListItem.h"

DEFINE_LOG_CATEGORY_STATIC(ScoreListLog, All, All);


void UScoreList::AddItem(FString _PlayerName, int32 ping)
{
	static int32 counter = 0;
	static FString BaseName = "Item";

	FString UniqueItemName = FString::Printf(TEXT("%s_%d"), *BaseName, counter);

	UE_LOG(ScoreListLog, Warning, TEXT("Name: %s"), *UniqueItemName);

	UScoreListItem* Item = CreateWidget<UScoreListItem>(GetOwningPlayer(), ItemClass, FName(UniqueItemName));

	Item->PlayerName->SetText(FText::FromString(_PlayerName));
	Item->Ping->SetText(ConvertToText(ping));

	ScrollBoxList->AddChild(Item);

	List.Add(Item);

	counter++;
}

void UScoreList::AddItem(FString _PlayerName, uint8 kill, uint8 death, uint8 ping)
{
	static int32 counter = 0;
	static FString BaseName = "Item";

	FString UniqueItemName = FString::Printf(TEXT("%s_%d"), *BaseName, counter);

	UE_LOG(ScoreListLog, Warning, TEXT("Name: %s"), *UniqueItemName);

	UScoreListItem* Item = CreateWidget<UScoreListItem>(GetOwningPlayer(), ItemClass, FName(UniqueItemName));

	Item->PlayerName->SetText(FText::FromString(_PlayerName));
	Item->Kill->SetText(ConvertToText(kill));
	Item->Death->SetText(ConvertToText(death));
	Item->Ping->SetText(ConvertToText(ping));

	ScrollBoxList->AddChild(Item);

	counter++;
}

void UScoreList::ClearItemList()
{
	ScrollBoxList->ClearChildren();
}

void UScoreList::UpdateItem(FString PlayerNameToUpdate, int32 kill, int32 death)
{
	UScoreListItem* Item = GetItemByPlayerName(PlayerNameToUpdate);
	if (Item)
	{
		if (kill != 0)
		{
			Item->Kill->SetText(FText::FromString(FString::FromInt(kill)));
		}
		if (death != 0)
		{
			Item->Kill->SetText(FText::FromString(FString::FromInt(death)));
		}
	}
}

void UScoreList::AddKill(FString _PlayerName)
{
	UScoreListItem* Item = GetItemByPlayerName(_PlayerName);
	if (Item)
	{
		int32 kills = ConvertToInt(Item->Kill->GetText());
		Item->Kill->SetText(ConvertToText(++kills));
	}
}

void UScoreList::AddDeath(FString _PlayerName)
{
	UScoreListItem* Item = GetItemByPlayerName(_PlayerName);
	if (Item)
	{
		int32 death = ConvertToInt(Item->Kill->GetText());
		Item->Death->SetText(ConvertToText(++death));
	}
}

void UScoreList::UpdatePing(FString _PlayerName, int32 ping)
{
	UScoreListItem* Item = GetItemByPlayerName(_PlayerName);
	if (Item)
	{
		Item->Ping->SetText(ConvertToText(ping));
	}
}

UScoreListItem* UScoreList::GetItemByPlayerName(FString _PlayerName)
{
	FText TextPlayerName = FText::FromString(_PlayerName);
	for (UScoreListItem* Item : List)
	{
		
		if (Item->PlayerName->GetText().EqualTo(TextPlayerName))
		{
			return Item;
		}
	}
	UE_LOG(ScoreListLog, Error, TEXT("GetItemByPlayerName return nullptr"));
	return nullptr;
}

int32 UScoreList::ConvertToInt(FText From)
{
	return FCString::Atoi(*From.ToString());
}

FText UScoreList::ConvertToText(int32 From)
{
	return FText::FromString(FString::FromInt(From));
}