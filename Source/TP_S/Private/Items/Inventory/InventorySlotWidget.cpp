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

	// 이미지 로딩
	if (IConImage && ItemData && !ItemData->Icon.IsNull())
	{
		// 간단히 직접 로드
		UTexture2D* LoadedIcon = ItemData->Icon.LoadSynchronous();
		if (LoadedIcon)
		{
			IConImage->SetBrushFromTexture(LoadedIcon);
			IConImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IConImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (IConImage)
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

void UInventorySlotWidget::HandleOnClicked()
{
	SetSelected(!bIsSelected);
	
	OnSlotClicked.Broadcast(this);
}
