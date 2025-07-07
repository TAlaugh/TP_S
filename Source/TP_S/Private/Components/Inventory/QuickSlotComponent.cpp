// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/QuickSlotComponent.h"

#include "AbilitySystemComponent.h"
#include "DebugHelper.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"

bool UQuickSlotComponent::RegisterItem(UConsumableItemDataAsset* Item, UPlayerInventoryComponent* Inventory)
{
	if (!Item || !Inventory) return false;
	
	const int32 Existing = Inventory->CountItem(Item);
	if (Existing <= 0) return false;

	QuickSlotData.ItemData = Item;
	QuickSlotData.Count = Existing;
	OnQuickSlotChanged.Broadcast(QuickSlotData);
	return true;
}

bool UQuickSlotComponent::UseSlot(UPlayerInventoryComponent* Inventory)
{
	if (!QuickSlotData.ItemData || !Inventory) return false;

	if (Inventory->RemoveOne(QuickSlotData.ItemData))
	{
		--QuickSlotData.Count;
		OnQuickSlotChanged.Broadcast(QuickSlotData);

		if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
		{
			if (auto* ASC = OwnerChar->FindComponentByClass<UAbilitySystemComponent>())
			{
				Debug::Print(TEXT("ASC->TryActivateAbilityByClass : QuickSlot.Item"));
				ASC->TryActivateAbilityByClass(QuickSlotData.ItemData->PrimaryAbility);
			}
		}

		if (QuickSlotData.Count <= 0)
		{
			QuickSlotData.ItemData = nullptr;
			OnQuickSlotChanged.Broadcast(QuickSlotData);
		}
		return true;
	}
	return false;
}
