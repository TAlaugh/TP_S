// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/InventoryMainWidget.h"

#include "DebugHelper.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"
#include "Items/Inventory/InventorySlotWidget.h"
#include "Items/Inventory/ItemInfoWidget.h"

void UInventoryMainWidget::Init(UPlayerInventoryComponent* InInventory)
{
	Inventory = InInventory;

	if (Inventory)
	{
		Inventory->OnInventoryChanged.AddDynamic(this, &UInventoryMainWidget::Refresh);
	}

	if (APawn* P = GetOwningPlayerPawn())
	{
		QuickSlotComponent = P->FindComponentByClass<UQuickSlotComponent>();
		if (QuickSlotComponent)
		{
			QuickSlotComponent->OnQuickSlotChanged.AddDynamic(this, &UInventoryMainWidget::HandleQuickSlotChanged);
		}
	}
	
	Refresh(); // 처음 로드
}

bool UInventoryMainWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (BtnConsumable)
	{
		BtnConsumable->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnConsumableTab);
	}

	if (BtnMelee)
	{
		BtnMelee->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnMeleeTab);
	}

	if (BtnRanged)
	{
		BtnRanged->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnRangedTab);
	}
	
	return true;
}

void UInventoryMainWidget::OnConsumableTab()
{
	CurrentTab = EInventoryCategory::Consumable;
	Refresh();
}

void UInventoryMainWidget::OnMeleeTab()
{
	CurrentTab = EInventoryCategory::Melee;
	Refresh();
}

void UInventoryMainWidget::OnRangedTab()
{
	CurrentTab = EInventoryCategory::Ranged;
	Refresh();
}

void UInventoryMainWidget::Refresh()
{
	// UE_LOG(LogTemp,Warning,TEXT("Refresh called"));
	
	if (!Inventory || !ScrollItems || !SlotClass)
	{
		// UE_LOG(LogTemp,Error,TEXT("Null ptr: ScrollItems=%s SlotClass=%s Inv=%s"), *GetNameSafe(ScrollItems), *GetNameSafe(SlotClass), *GetNameSafe(Inventory));
		return;
	}

	ScrollItems->ClearChildren();

	if (InfoWidgetHolder)
	{
		InfoWidgetHolder->ClearChildren();
		CurrentInfoWidget = nullptr;
	}
	
	UItemDataAsset* QuickItem = QuickSlotComponent ? QuickSlotComponent->GetData().ItemData : nullptr;
	
	const TArray<FItemStack>& Items = Inventory->GetStacks();
	// UE_LOG(LogTemp,Warning,TEXT("Stacks = %d"), Items.Num());
	
	for (const FItemStack& Stack : Items)
	{
		// UE_LOG(LogTemp,Warning,TEXT("item %s cat %d cnt %d"), *Stack.ItemData->GetName(), int32(Stack.ItemData->Category), Stack.Count);
		
		if (CurrentTab != EInventoryCategory::None && Stack.ItemData->Category != CurrentTab) continue;

		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotClass);
		SlotWidget->SetupSlot(Stack.ItemData, Stack.Count);
		
		const bool bIsQuick = (QuickItem && Stack.ItemData == QuickItem);
		SlotWidget->SetQuickSlotBG(bIsQuick);
		
		SlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryMainWidget::HandleSlotClicked);
		ScrollItems->AddChild(SlotWidget);

		SlotWidget->OnItemClicked.AddDynamic(this, &UInventoryMainWidget::ShowItemInfo);
	}
}

void UInventoryMainWidget::HandleSlotClicked(UInventorySlotWidget* Clicked)
{
	if (!Clicked) return;
	
	SetSelectedSlot(Clicked);

	UItemDataAsset* Item = Clicked->GetItem();
	if (!Item) return;

	if (Item->Category == EInventoryCategory::Consumable)
	{
		if (APawn* P = GetOwningPlayerPawn())
		{
			if (auto* QSC = P->FindComponentByClass<UQuickSlotComponent>())
			{
				if (auto* Inv = P->FindComponentByClass<UPlayerInventoryComponent>())
				{
					QSC->RegisterItem(Cast<UConsumableItemDataAsset>(Item), Inv);
					// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, (TEXT("%s"), QSC->GetData().ItemData.GetName()));
				}
			}
		}
	}
	else if (Item->Category == EInventoryCategory::Melee || Item->Category == EInventoryCategory::Ranged)
	{
		// TODO : 무기 장착 / 해제 로직 구현
	}
}

void UInventoryMainWidget::SetSelectedSlot(UInventorySlotWidget* NewSlot)
{
	if (SelectedSlot && SelectedSlot != NewSlot)
	{
		SelectedSlot->SetSelected(false);
	}
	
	SelectedSlot = NewSlot;
	
	if (SelectedSlot)
	{
		SelectedSlot->SetSelected(true);
	}
}

void UInventoryMainWidget::HandleQuickSlotChanged(const FQuickSlotData& Data)
{
	Refresh();
}

void UInventoryMainWidget::ShowItemInfo(UItemDataAsset* ItemData)
{
	if (!ItemData || !InfoWidgetHolder || !InfoWidgetClass) return;

	if (CurrentInfoWidget)
	{
		CurrentInfoWidget->RemoveFromParent();
		CurrentInfoWidget = nullptr;
	}

	CurrentInfoWidget = CreateWidget<UItemInfoWidget>(this, InfoWidgetClass);
	CurrentInfoWidget->ItemData = ItemData;

	InfoWidgetHolder->AddChild(CurrentInfoWidget);
}
