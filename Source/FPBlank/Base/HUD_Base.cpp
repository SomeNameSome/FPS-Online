#include "HUD_Base.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "../Widget/CharacterInfo.h"
#include "../Widget/WeaponInfo.h"
#include "../FreeBattle/PlayerState_FB.h"
#include "../Widget/ScoreList.h"
#include "../Widget/InventoryWidget.h"
#include <FPBlank/Primary/CharacterPrimary.h>

DEFINE_LOG_CATEGORY_STATIC(HUDLog, All, All);

AHUD_Base::AHUD_Base()
{
}

void AHUD_Base::BeginPlay()
{
	Super::BeginPlay();
	if (!WidgetsAlreadyExist())
		CreateWidgets();
}

UUserWidget* AHUD_Base::GetWidgetFromEnum(EWidgetList WidgetEnum) const
{
	switch (WidgetEnum)
	{
	case EWidgetList::CharacterInfoWidget:
	{
		return CharacterInfoWidget;
		break;
	}
	case EWidgetList::WeaponInfoWidget:
	{
		return WeaponInfoWidget;
		break;
	}
	case EWidgetList::PauseMenuWidget:
	{
		return PauseMenuWidget;
		break;
	}
	case EWidgetList::ScoreWidget:
	{
		return ScoreListWidget;
		break;
	}
	case EWidgetList::InventoryWidget:
	{
		return InventoryWidget;
		break;
	}
	}
	return CreateWidget<UUserWidget>(GetOwningPlayerController());
}

void AHUD_Base::AddWidgetToViewport(EWidgetList WidgetToAdd, int32 ZOrder)
{
	if (!WidgetInViewport(WidgetToAdd))
		GetWidgetFromEnum(WidgetToAdd)->AddToViewport(ZOrder);
}

void AHUD_Base::RemoveWidgetFromViewport(EWidgetList WidgetToRemove)
{
	if (WidgetInViewport(WidgetToRemove))
		GetWidgetFromEnum(WidgetToRemove)->RemoveFromParent();
}

bool AHUD_Base::WidgetInViewport(EWidgetList WidgetToCheck)
{
	return GetWidgetFromEnum(WidgetToCheck)->IsInViewport();
}

void AHUD_Base::SetWidgetVisibility(EWidgetList Widget, bool visible)
{
	UUserWidget* handleWidget = GetWidgetFromEnum(Widget);
	if (handleWidget->IsInViewport())
	{
		ESlateVisibility visibility;
		visible ? visibility = ESlateVisibility::Visible : visibility = ESlateVisibility::Hidden;
		handleWidget->SetVisibility(visibility);
	}
}

void AHUD_Base::SetHealthBarValue(float NewValue)
{
	NewValue /= 100.f;
	CharacterInfoWidget->SetHealthBarValue(NewValue);
}

bool AHUD_Base::WidgetIsVisible(EWidgetList Widget) const
{
	return GetWidgetFromEnum(Widget)->IsVisible();
}

void AHUD_Base::UpdateHealtBarValue(float UpdatingValue)
{
	CharacterInfoWidget->UpdateHealtBarValue(UpdatingValue);
}

void AHUD_Base::PlayHitMarker(bool isHeadshot)
{
	WeaponInfoWidget->PlayHitMarkerAnimation(isHeadshot);
}

void AHUD_Base::SetCrosshairVisibility(bool visible)
{
	WeaponInfoWidget->SetCrosshairVisibility(visible);
}

void AHUD_Base::SetAmmoInMagazine(int32 currentAmmo)
{
	WeaponInfoWidget->SetAmmoInMagazine(currentAmmo);
}

void AHUD_Base::SetInteractionWidgetState(FName InteractionInfo)
{
	CharacterInfoWidget->SetInteractionWidgetState(InteractionInfo);
}

void AHUD_Base::SetKeyboardFocusOnWidget(EWidgetList Widget)
{
	GetWidgetFromEnum(Widget)->SetKeyboardFocus();
}

void AHUD_Base::CreateWidgets()
{
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController)
	{
		UE_LOG(HUDLog, Warning, TEXT("Creating Widgets!!!"));
		if (!CharacterInfoWidget)
			CharacterInfoWidget = CreateWidget<UCharacterInfo>(playerController, CharacterInfoWidgetClass, FName("CharacterInfoWidget"));
		if (!WeaponInfoWidget)
			WeaponInfoWidget = CreateWidget<UWeaponInfo>(playerController, WeaponInfoWidgetClass, FName("WeaponInfoWidget"));
		if (!PauseMenuWidget)
			PauseMenuWidget = CreateWidget<UUserWidget>(playerController, PauseMenuWidgetClass, FName("PauseMenuWidget"));
		if (!ScoreListWidget)
			ScoreListWidget = CreateWidget<UScoreList>(playerController, ScoreListWidgetClass, FName("ScoreListWidget"));
		if (!InventoryWidget)
			InventoryWidget = CreateWidget<UInventoryWidget>(playerController, InventoryWidgetClass, FName("InventoryWidget"));

		if (CharacterInfoWidget)
			CharacterInfoWidget->AddToViewport();
		if (InventoryWidget)
			InventoryWidget->BindDelegates(Cast<ACharacterPrimary>(GetOwningPawn())->GetInventoryComponent());
	}
}

bool AHUD_Base::WidgetsAlreadyExist()
{
	return CharacterInfoWidget && WeaponInfoWidget;
}

void AHUD_Base::CreateScoreList(const TArray<APlayerState*>& PS_Array)
{
	ScoreListWidget->ClearItemList();
	for (APlayerState* PS_Iter : PS_Array)
	{
		APlayerState_FB* PS = Cast<APlayerState_FB>(PS_Iter);
		if (PS)
			ScoreListWidget->AddItem(PS->GetPlayerName(), PS->GetKill(), PS->GetDeath(), PS->GetCompressedPing());
		else
			UE_LOG(HUDLog, Error, TEXT("!PS"));
	}
	ScoreListWidget->AddToViewport();
}

void AHUD_Base::AddItemToWidgetInventory(UTexture2D* ItemIcon)
{
	InventoryWidget->AddWeapon(ItemIcon);
}

void AHUD_Base::Reset()
{
	SetHealthBarValue(100.f);
}