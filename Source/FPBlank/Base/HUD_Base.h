#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Base.generated.h"


UENUM(BlueprintType)
enum class EWidgetList : uint8
{
	CharacterInfoWidget,
	WeaponInfoWidget,
	PauseMenuWidget,
	InventoryWidget,
	ScoreWidget
};

class UCharacterInfo;
class UWeaponInfo;
class UScoreList;
class UInventoryWidget;

UCLASS()
class FPBLANK_API AHUD_Base : public AHUD
{
	GENERATED_BODY()

protected:
	AHUD_Base();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<UCharacterInfo> CharacterInfoWidgetClass;

	UPROPERTY();
	UCharacterInfo* CharacterInfoWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<UWeaponInfo> WeaponInfoWidgetClass;

	UPROPERTY();
	UWeaponInfo* WeaponInfoWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly);
	UUserWidget* PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<UScoreList> ScoreListWidgetClass;

	UPROPERTY();
	UScoreList* ScoreListWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY();
	UInventoryWidget* InventoryWidget;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(EWidgetList Widget, bool visible);

	bool WidgetIsVisible(EWidgetList Widget) const;

	void AddWidgetToViewport(EWidgetList WidgetToAdd, int32 ZOrder = 0);

	void RemoveWidgetFromViewport(EWidgetList WidgetToRemove);

	bool WidgetInViewport(EWidgetList WidgetToCheck);

	void SetHealthBarValue(float NewValue);

	void UpdateHealtBarValue(float UpdatingValue);

	void PlayHitMarker(bool isHeadshot);

	void SetCrosshairVisibility(bool visible);

	void SetAmmoInMagazine(int32 currentAmmo);

	void SetInteractionWidgetState(FName InteractionInfo);

	void SetKeyboardFocusOnWidget(EWidgetList Widget);

	virtual void CreateWidgets();

	bool WidgetsAlreadyExist();

	void CreateScoreList(const TArray<APlayerState*>& PS_Array);

	UInventoryWidget* GetInventoryWidget() { return InventoryWidget; }

	void Reset() override;

	void AddItemToWidgetInventory(UTexture2D* ItemIcon);

protected:
	//potentially-throwing
	virtual UUserWidget* GetWidgetFromEnum(EWidgetList WidgetEnum) const;
};
