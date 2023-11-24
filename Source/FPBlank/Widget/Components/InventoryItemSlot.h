#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemSlot.generated.h"




class UVerticalBox;
class UBorder;
class UImage;
class UInventoryComponent;



UCLASS()
class FPBLANK_API UItemDragDrop : public UDragDropOperation
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (ExposeOnSpawn), BlueprintReadOnly);
	UImage* ItemIcon;
	UPROPERTY(meta = (ExposeOnSpawn), BlueprintReadOnly);
	int32 itemIndex;
};


UCLASS()
class FPBLANK_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(meta = (BindWidget));
	UVerticalBox* Container;

	UPROPERTY(meta = (BindWidget));
	UBorder* Delimiter;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly);
	UImage* ItemIcon;

	UPROPERTY(BlueprintReadOnly);
	int32 index;

private:
	FVector2D WeaponIconSize;

public:
	bool IsFree();
	void SetIcon(UTexture2D* NewIcon);
	FORCEINLINE UImage* GetIcon() { return ItemIcon; }
	FORCEINLINE void SetIndex(uint8 _index) { index = _index; }

protected:
	void NativeOnInitialized() override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable)
		void RemoveItem();

private:
	void SetItemIconVisibility(bool visible);
	void CallWeaponSwapDelegate(int32 firstIndex, int32 secondIndex);
	void CallWeaponRemoveDelegate(int32 _index);
};
