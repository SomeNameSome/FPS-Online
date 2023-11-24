#include "CharacterInfo.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "InteractionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(Widget_CharacterInfoLog, All, All);

void UCharacterInfo::SetHealthBarValue(float NewValue)
{
	UE_LOG(Widget_CharacterInfoLog, Warning, TEXT("Health: %f"), NewValue);
	HealthBar->SetPercent(NewValue);
}

void UCharacterInfo::UpdateHealtBarValue(float UpdatingValue)
{
	float percent = HealthBar->GetPercent() + UpdatingValue;
	HealthBar->SetPercent(percent);
	UE_LOG(Widget_CharacterInfoLog, Warning, TEXT("Value: %f"), HealthBar->GetPercent());
}

void UCharacterInfo::SetInteractionWidgetState(FName _InteractionInfo)
{
	if (_InteractionInfo.IsNone())
		InteractionWidget->Remove();
	else
		InteractionWidget->Add(_InteractionInfo);
}