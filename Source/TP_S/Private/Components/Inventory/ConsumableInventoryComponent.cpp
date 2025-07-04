// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/ConsumableInventoryComponent.h"

#include "DebugHelper.h"
#include "Items/Consumables/ConsumableItem.h"

void UConsumableInventoryComponent::AddItem(UConsumableItem* Item, int32 Amount)
{
	if (!Item || Amount <= 0) return;

	for (FConsumableInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemData == Item)
		{
			Slot.Amount += Amount;
			OnInventoryUpdated.Broadcast();
			return;
		}
	}

	FConsumableInventorySlot NewSlot;
	NewSlot.ItemData = Item;
	NewSlot.Amount = Amount;
	InventorySlots.Add(NewSlot);

	OnInventoryUpdated.Broadcast();
}

void UConsumableInventoryComponent::ConsumeItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex)) return;

	FConsumableInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Amount > 0)
	{
		--Slot.Amount;
		Slot.CooldownEndTime = GetWorld()->GetTimeSeconds() + Slot.ItemData->CooldownTime;

		if (Slot.Amount == 0)
		{
			InventorySlots.RemoveAt(SlotIndex);
		}

		OnInventoryUpdated.Broadcast();
	}
}

bool UConsumableInventoryComponent::IsOnCooldown(const FConsumableInventorySlot& Slot) const
{
	return GetWorld()->GetTimeSeconds() < Slot.CooldownEndTime;
}

const FConsumableInventorySlot& UConsumableInventoryComponent::GetSlot(int32 Index) const
{
	return InventorySlots[Index];
}

bool UConsumableInventoryComponent::IsValidIndex(int32 Index) const
{
	return InventorySlots.IsValidIndex(Index);
}
