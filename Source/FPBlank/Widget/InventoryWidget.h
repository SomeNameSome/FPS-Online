#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"




DECLARE_DELEGATE_TwoParams(FWeaponSwapDelegate, int32 firstIndex, int32 secondIndex)
DECLARE_DELEGATE_OneParam(FWeaponRemoveDelegate, int32 itemIndex)

class UInventoryItemSlot;
class UCanvasPanel;
class UBorder;
class UVerticalBox;
class UInventoryComponent;


UCLASS()
class FPBLANK_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	FWeaponSwapDelegate OnWeaponSwapDelegate;
	FWeaponRemoveDelegate OnWeaponRemoveDelegate;

protected:
	UPROPERTY(meta = (BindWidget));
	UCanvasPanel* _CanvasPanel;

	UPROPERTY(meta = (BindWidget));
	UBorder* _Border;

	UPROPERTY(meta = (BindWidget));
	UVerticalBox* WeaponContainer;

	UPROPERTY(meta = (BindWidget));
	UInventoryItemSlot* PrimaryWeaponItem;

	UPROPERTY(meta = (BindWidget));
	UInventoryItemSlot* SecondaryWeaponItem;

	UPROPERTY(meta = (BindWidget));
	UInventoryItemSlot* TertiaryWeaponItem;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly);
	TArray<UInventoryItemSlot*> WeaponItems;


public:
	void AddWeapon(UTexture2D* WeaponIcon);
	void SwapWeapons(int32 firstIndex, int32 secondIndex);
	void BindDelegates(UInventoryComponent* Inventory);
	

protected:
	void NativeOnInitialized() override;
	void NativePreConstruct() override;
	void NativeDestruct() override;

private:
	UInventoryItemSlot* FindFreeSlot();
};
