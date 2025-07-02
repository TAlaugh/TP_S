// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/BaseQuickSlotComponent.h"

#include "DebugHelper.h"
#include "Components/Inventory/ConsumableInventoryComponent.h"

void UBaseQuickSlotComponent::Initialize(UConsumableInventoryComponent* InConsumableInventoryComponent)
{
	InventoryComponent = InConsumableInventoryComponent;
}

void UBaseQuickSlotComponent::EquipItemByIndex(int32 Index)
{
	if (!InventoryComponent || !InventoryComponent->IsValidIndex(Index))
	{
		EquippedSlotIndex = INDEX_NONE;
		return;
	}

	EquippedSlotIndex = Index;
	OnQuickSlotChanged.Broadcast();
}

void UBaseQuickSlotComponent::UseEquippedItem()
{
	if (!InventoryComponent || !InventoryComponent->IsValidIndex(EquippedSlotIndex)) return;

	const FConsumableInventorySlot& Slot = InventoryComponent->GetSlot(EquippedSlotIndex);

	if (Slot.Amount <= 0 || InventoryComponent->IsOnCooldown(Slot)) return;

	// TODO : AbilitySystem 연동
	Debug::Print(TEXT("BaseQuickSlotComponent.cpp->UseEquippedItem(), AbilitySystem 연동"));
	
	InventoryComponent->ConsumeItem(EquippedSlotIndex);
}
