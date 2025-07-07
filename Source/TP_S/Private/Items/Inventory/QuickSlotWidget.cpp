// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/QuickSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"

void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* P = GetOwningPlayerPawn())
	{
		if (auto* QuickSlotComponent = P->FindComponentByClass<UQuickSlotComponent>())
		{
			QuickSlotComponent->OnQuickSlotChanged.AddDynamic(this, &UQuickSlotWidget::Update);
			Update(QuickSlotComponent->GetData());
		}
	}
}

void UQuickSlotWidget::Update(const FQuickSlotData& Data)
{
	if (ImageIcon)
	{
		if (Data.ItemData)
		{
			if (!Data.ItemData->IconMaterial.IsNull())
			{
				ImageIcon->SetBrushFromMaterial(Data.ItemData->IconMaterial.LoadSynchronous());
			}
			else
			{
				ImageIcon->SetBrushFromTexture(Data.ItemData->IconTexture.LoadSynchronous());
			}
			ImageIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ImageIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (TxtCount)
	{
		const bool bShow = Data.Count > 1;
		TxtCount->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (bShow)
		{
			TxtCount->SetText(FText::AsNumber(Data.Count));
		}
	}

	
}
