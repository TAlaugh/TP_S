// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Consumables/ConsumableItemDataAsset.h"

UConsumableItemDataAsset::UConsumableItemDataAsset()
{
	Category = EInventoryCategory::Consumable;
	MaxAmount = 10;
}
