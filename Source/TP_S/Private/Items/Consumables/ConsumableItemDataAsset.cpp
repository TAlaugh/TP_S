// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Consumables/ConsumableItemDataAsset.h"

UConsumableItemDataAsset::UConsumableItemDataAsset()
{
	Category = EInventoryCategory::Consumable;
	MaxAmount = 10;
}

void UConsumableItemDataAsset::GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const
{
	Super::GatherDisplayRows(OutRows);

	if (!FMath::IsNearlyZero(HpPercent))
	{
		OutRows.Add({NSLOCTEXT("UI", "HealPercent", "HP Percent 회복"), FText::Format(NSLOCTEXT("UI", "Heal Percent", "{0}%"), HpPercent * 100)});
	}
	if (HpFloat != 0)
	{
		OutRows.Add({NSLOCTEXT("UI", "HealFloat", "HP Float 회복"), FText::Format(NSLOCTEXT("UI", "Heal Float", "{0}"), HpFloat)});
	}
	if (AtkFloat != 0)
	{
		OutRows.Add({NSLOCTEXT("UI", "AtkFloat", "Atk Float 상승"), FText::Format(NSLOCTEXT("UI", "Atk Float", "{0}"), AtkFloat)});
	}
}
