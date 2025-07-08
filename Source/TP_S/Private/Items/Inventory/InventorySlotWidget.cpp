// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/InventorySlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/ItemDataAsset.h"

void UInventorySlotWidget::SetupSlot(UItemDataAsset* InItemData, int32 InCount)
{
	ItemData = InItemData;
	Count = InCount;

	if (!IConImage) return;
	
	/** 머테리얼이 있으면 머테리얼 없으면 텍스쳐 **/
	if (ItemData && !ItemData->IconMaterial.IsNull())
	{
		UMaterialInterface* MI = ItemData->IconMaterial.LoadSynchronous();
		IConImage->SetBrushFromMaterial(MI);
		IConImage->SetVisibility(ESlateVisibility::Visible);
	}
	else if (ItemData && !ItemData->IconTexture.IsNull() && ItemData->IconMaterial.IsNull())
	{
		UTexture2D* Texture = ItemData->IconTexture.LoadSynchronous();
		IConImage->SetBrushFromTexture(Texture);
		IConImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IConImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 수량 표시
	if (CountText)
	{
		if (ItemData && ItemData->Category == EInventoryCategory::Consumable)
		{
			CountText->SetText(FText::AsNumber(Count));
			CountText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		CountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UInventorySlotWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::HandleOnClicked);
		SetSelected(bIsSelected);
	}

	return true;
}

void UInventorySlotWidget::SetSelected(bool bNewSelected)
{
	bIsSelected = bNewSelected;

	if (Selected_BG && Circle_Selected_BG)
	{
		Selected_BG->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		Circle_Selected_BG->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UInventorySlotWidget::SetQuickSlotBG(bool bOn)
{
	if (QuickSlot_BG)
	{
		QuickSlot_BG->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UInventorySlotWidget::HandleOnClicked()
{
	SetSelected(!bIsSelected);
	
	OnSlotClicked.Broadcast(this);
}
