#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInfo.generated.h"


class UCanvasPanel;
class UTextBlock;
class UImage;
class UHorizontalBox;


UCLASS()
class FPBLANK_API UWeaponInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget));
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* AmmoInMagazine;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* TotalAmmo;

	UPROPERTY(meta = (BindWidget));
	UTextBlock* Delimiter;

	UPROPERTY(meta = (BindWidget));
	UImage* Crosshair;

	UPROPERTY(meta = (BindWidget));
	UImage* HitMarker;

	UPROPERTY(meta = (BindWidget));
	UHorizontalBox* BoxAmmoInfo;

	UPROPERTY(Transient, meta = (BindWidgetAnim));
	UWidgetAnimation* ShotAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim));
	UWidgetAnimation* HeadshotAnimation;

public:
	void PlayHitMarkerAnimation(bool headshot);

	void SetCrosshairVisibility(bool visible);

	void SetAmmoInMagazine(int32 currentAmmo);
};
