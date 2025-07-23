// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemAcquiredWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemAcquiredWidget::InitItemWidget(FText ItemName, UMaterialInterface* ItemIcon)
{
	if (ItemText)
	{
		ItemText->SetText(ItemName);
	}

	if (ItemImage && ItemIcon)
	{
		ItemImage->SetBrushFromMaterial(ItemIcon);
	}
}