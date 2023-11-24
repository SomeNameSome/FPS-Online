#include "InventoryItemSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "../InventoryWidget.h"
#include "Blueprint/WidgetTree.h"


DEFINE_LOG_CATEGORY_STATIC(InventoryItemSlotLog, All, All);

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeConstruct();
	WeaponIconSize.Set(460.f, 160.f);
	ItemIcon->SetDesiredSizeOverride(WeaponIconSize);
	ItemIcon->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;
}

bool UInventoryItemSlot::IsFree()
{
	return ItemIcon->GetBrush().GetDrawType() != ESlateBrushDrawType::Image;
}

void UInventoryItemSlot::SetIcon(UTexture2D* NewIcon)
{
	UE_LOG(InventoryItemSlotLog, Warning, TEXT("SetIcon"));
	ItemIcon->Brush.DrawAs = ESlateBrushDrawType::Image;
	ItemIcon->SetBrushFromTexture(NewIcon);
}

void UInventoryItemSlot::SetItemIconVisibility(bool visible)
{
	FLinearColor color(FLinearColor::White);
	visible ? color.A = 1 : color.A = 0;

	ItemIcon->SetColorAndOpacity(color);
	UE_LOG(InventoryItemSlotLog, Warning, TEXT("Color: %s"), *color.ToString());
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(InventoryItemSlotLog, Warning, TEXT("NativeOnDrop"));
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UItemDragDrop* OtherSlot = Cast<UItemDragDrop>(InOperation);
	if (OtherSlot)
	{
		if (ItemIcon->GetBrush().GetDrawType() == ESlateBrushDrawType::Image) // not empty slot
		{
			UObject* OtherItemTexture = OtherSlot->ItemIcon->GetBrush().GetResourceObject();
			UObject* OurItemTexture = ItemIcon->GetBrush().GetResourceObject();
			OtherSlot->ItemIcon->SetBrushResourceObject(OurItemTexture);
			ItemIcon->SetBrushResourceObject(OtherItemTexture);
		}
		else // empty slot
		{
			UObject* OtherItemTexture = OtherSlot->ItemIcon->GetBrush().GetResourceObject();
			ItemIcon->Brush.DrawAs = ESlateBrushDrawType::Image;
			ItemIcon->SetBrushResourceObject(OtherItemTexture);
			OtherSlot->ItemIcon->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;
		}
		CallWeaponSwapDelegate(this->index, OtherSlot->itemIndex);
	}
	
	return true;
}

void UInventoryItemSlot::CallWeaponSwapDelegate(int32 firstIndex, int32 secondIndex)
{
	UInventoryWidget* Owner = Cast<UInventoryWidget>(GetOuter()->GetOuter());
	if (Owner)
	{
		if (Owner->OnWeaponSwapDelegate.IsBound())
		{
			UE_LOG(InventoryItemSlotLog, Warning, TEXT("OnWeaponSwapDelegate.IsBound()"));
			Owner->OnWeaponSwapDelegate.Execute(firstIndex, secondIndex);
		}
		else
		{
			UE_LOG(InventoryItemSlotLog, Warning, TEXT("!OnWeaponSwapDelegate.IsBound()"));
		}
	}
}

void UInventoryItemSlot::CallWeaponRemoveDelegate(int32 _index)
{
	UInventoryWidget* Owner = Cast<UInventoryWidget>(GetOuter()->GetOuter());
	if (Owner)
	{
		if (Owner->OnWeaponRemoveDelegate.IsBound())
		{
			UE_LOG(InventoryItemSlotLog, Warning, TEXT("OnWeaponSwapDelegate.IsBound()"));
			Owner->OnWeaponRemoveDelegate.Execute(_index);
		}
		else
		{
			UE_LOG(InventoryItemSlotLog, Warning, TEXT("!OnWeaponSwapDelegate.IsBound()"));
		}
	}
}

void UInventoryItemSlot::RemoveItem()
{
	ItemIcon->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;
	CallWeaponRemoveDelegate(index);
}