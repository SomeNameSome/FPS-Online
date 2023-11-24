#include "WeaponInfo.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Animation/WidgetAnimation.h"

void UWeaponInfo::PlayHitMarkerAnimation(bool headshot)
{
	headshot ? PlayAnimation(HeadshotAnimation) : PlayAnimation(ShotAnimation);
}

void UWeaponInfo::SetCrosshairVisibility(bool visible)
{
	visible ? Crosshair->SetVisibility(ESlateVisibility::Visible) : Crosshair->SetVisibility(ESlateVisibility::Hidden);
}

void UWeaponInfo::SetAmmoInMagazine(int32 currentAmmo)
{
	AmmoInMagazine->SetText(FText::FromString(FString::FromInt(currentAmmo)));
}