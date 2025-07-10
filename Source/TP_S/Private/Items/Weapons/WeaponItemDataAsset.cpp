// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponItemDataAsset.h"

UWeaponItemDataAsset::UWeaponItemDataAsset()
{
	Category = EInventoryCategory::Melee;
	MaxAmount = 1;
}

void UWeaponItemDataAsset::GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const
{
	// Super::GatherDisplayRows(OutRows);

	if (!FMath::IsNearlyZero(BaseDamage))
	{
		OutRows.Add({NSLOCTEXT("UI", "BaseDamage", "Base Damage"), FText::Format(NSLOCTEXT("UI", "Base Damage", "{0}"), BaseDamage)});
	}
}
