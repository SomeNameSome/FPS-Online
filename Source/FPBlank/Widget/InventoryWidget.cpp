#include "InventoryWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/InventoryItemSlot.h"
#include "../Base/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"


DEFINE_LOG_CATEGORY_STATIC(InventoryWidgetLog, All, All);

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	WeaponItems.Add(PrimaryWeaponItem);
	WeaponItems.Add(SecondaryWeaponItem);
	WeaponItems.Add(TertiaryWeaponItem);
	PrimaryWeaponItem->SetIndex(0);
	SecondaryWeaponItem->SetIndex(1);
	TertiaryWeaponItem->SetIndex(2);
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
	}
}

void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently);
	}
}

void UInventoryWidget::AddWeapon(UTexture2D* WeaponIcon)
{
	UE_LOG(InventoryWidgetLog, Display, TEXT("UInventoryWidget::AddWeapon"));
	UInventoryItemSlot* ItemSlot = FindFreeSlot();
	if (ItemSlot)
		ItemSlot->SetIcon(WeaponIcon);
	else
		UE_LOG(InventoryWidgetLog, Error, TEXT("!ItemSlot"))
}

UInventoryItemSlot* UInventoryWidget::FindFreeSlot()
{
	for (UInventoryItemSlot* Current : WeaponItems)
	{
		if (Current->IsFree())
		{
			return Current;
		}
	}
	return nullptr;
}

void UInventoryWidget::BindDelegates(UInventoryComponent* Inventory)
{
	OnWeaponSwapDelegate.BindUFunction(Inventory, "SwapWeaponServer");
	OnWeaponRemoveDelegate.BindUObject(Inventory, &UInventoryComponent::RemoveItem);
}

void UInventoryWidget::SwapWeapons(int32 firstIndex, int32 secondIndex)
{
	UImage* firstIcon = WeaponItems[firstIndex]->GetIcon();
	UImage* secondIcon = WeaponItems[secondIndex]->GetIcon();
	UImage* EmptySlot;
	UImage* NotEmptySlot;
	if (firstIcon->GetBrush().GetDrawType() == ESlateBrushDrawType::Image)
	{
		NotEmptySlot = firstIcon;
		EmptySlot = secondIcon;
	}
	else
	{
		NotEmptySlot = secondIcon;
		EmptySlot = firstIcon;
	}
	UObject* IconTexture = NotEmptySlot->GetBrush().GetResourceObject();
	EmptySlot->Brush.DrawAs = ESlateBrushDrawType::Image;
	EmptySlot->SetBrushResourceObject(IconTexture);
	NotEmptySlot->Brush.DrawAs = ESlateBrushDrawType::NoDrawType;
}