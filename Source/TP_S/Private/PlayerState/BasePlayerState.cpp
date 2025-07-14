// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BasePlayerState.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "GameFramework/Character.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"
#include "Net/UnrealNetwork.h"

ABasePlayerState::ABasePlayerState()
{
	bReplicates = true;
}

void ABasePlayerState::CaptureFromComponents(ABasePlayerCharacter* PlayerChar)
{
	if (auto* Inv = PlayerChar->FindComponentByClass<UPlayerInventoryComponent>())
	{
		StoredInventory.Reset();
		for (const FItemStack& Stack : Inv->GetAllItemStacks())
		{
			StoredInventory.Add({ Stack.ItemData, Stack.Count });
		}
	}

	if (auto* Quick = PlayerChar->FindComponentByClass<UQuickSlotComponent>())
	{
		const auto& SlotData = Quick->GetQuickSlotData();
		StoredQuickSlot.ItemData = SlotData.ItemData;
		StoredQuickSlot.Count = SlotData.Count;
	}

	if (auto* CombatComp = PlayerChar->FindComponentByClass<UBasePlayerCombatComponent>())
	{
		StoredCombatInfo.MeleeWeaponTag = CombatComp->GetEquippedMeleeTag();
		StoredCombatInfo.RangeWeaponTag = CombatComp->GetEquippedRangeTag();
	}

	if (UBaseAbilitySystemComponent* ASC = PlayerChar->FindComponentByClass<UBaseAbilitySystemComponent>())
	{
		if (const UBaseAttributeSet* AS = ASC->GetSet<UBaseAttributeSet>())
		{
			StoredAttributes.CurrentHp = AS->GetCurrentHp();
			StoredAttributes.MaxHp = AS->GetMaxHp();
		}
	}
}

void ABasePlayerState::RestoreToComponents(ABasePlayerCharacter* PlayerChar)
{
	if (auto* Inv = PlayerChar->FindComponentByClass<UPlayerInventoryComponent>())
	{
		Inv->ClearItems();
		for (const FStoredItem& Stack : StoredInventory)
		{
			Inv->AddItem(Stack.ItemData.Get(), Stack.Quantity);
		}
	}

	if (auto* Quick = PlayerChar->FindComponentByClass<UQuickSlotComponent>())
	{
		if (auto* Inv = PlayerChar->FindComponentByClass<UPlayerInventoryComponent>())
		{
			Quick->RegisterItem(Cast<UConsumableItemDataAsset>(StoredQuickSlot.ItemData.Get()), Inv);
		}
	}

	if (auto* Combat = PlayerChar->FindComponentByClass<UBasePlayerCombatComponent>())
	{
		if (StoredCombatInfo.MeleeWeaponTag.IsValid())
		{
			Combat->EquipWeaponFromInventory(nullptr, StoredCombatInfo.MeleeWeaponTag);
		}
		if (StoredCombatInfo.RangeWeaponTag.IsValid())
		{
			Combat->EquipWeaponFromInventory(nullptr, StoredCombatInfo.RangeWeaponTag);
		}
	}

	if (UAbilitySystemComponent* ASC = PlayerChar->FindComponentByClass<UAbilitySystemComponent>())
	{
		if (const UBaseAttributeSet* AS = ASC->GetSet<UBaseAttributeSet>())
		{
			ASC->SetNumericAttributeBase(UBaseAttributeSet::GetCurrentHpAttribute(), StoredAttributes.CurrentHp);
			ASC->SetNumericAttributeBase(UBaseAttributeSet::GetMaxHpAttribute(), StoredAttributes.MaxHp);
		}
	}
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlayerState, StoredInventory);
	DOREPLIFETIME(ABasePlayerState, StoredQuickSlot);
	DOREPLIFETIME(ABasePlayerState, StoredCombatInfo);
	DOREPLIFETIME(ABasePlayerState, StoredAttributes);
	DOREPLIFETIME(ABasePlayerState, bShouldRestoreData);
}
