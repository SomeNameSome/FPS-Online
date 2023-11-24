#include "InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"



void UInteractionWidget::Add(FName _InteractionInfo)
{
	InteractionInfo->SetText(FText::FromName(_InteractionInfo));
	if (!IsVisible())
		SetVisibility(ESlateVisibility::Visible);
}

void UInteractionWidget::Remove()
{
	if (IsVisible())
		SetVisibility(ESlateVisibility::Hidden);
}