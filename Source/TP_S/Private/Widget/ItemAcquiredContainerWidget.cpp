// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemAcquiredContainerWidget.h"

#include "Components/VerticalBox.h"
#include "Items/ItemDataAsset.h"
#include "Widget/ItemAcquiredWidget.h"

void UItemAcquiredContainerWidget::AddAcquiredItem(UItemDataAsset* ItemDataAsset, int32 Amount)
{
	if (!ItemDataAsset || !ItemWidgetClass || !BoxItems) return;
	
	UItemAcquiredWidget* NewItemWidget = CreateWidget<UItemAcquiredWidget>(this, ItemWidgetClass);
	if (!NewItemWidget) return;
	
	FText DisplayName = FText::FromString(FString::Printf(TEXT("%s x%d"), *ItemDataAsset->DisplayName.ToString(), Amount));
	NewItemWidget->InitItemWidget(DisplayName, ItemDataAsset->IconMaterial.Get());

	BoxItems->AddChildToVerticalBox(NewItemWidget);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewItemWidget]()
	{
		if (NewItemWidget && NewItemWidget->IsValidLowLevel())
		{
			NewItemWidget->RemoveFromParent();
		}
	}, 2.f, false);
}