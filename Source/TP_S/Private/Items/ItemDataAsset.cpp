// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemDataAsset.h"

void UItemDataAsset::GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const
{
	OutRows.Add({NSLOCTEXT("UI", "MaxAmount", "최대 수"), FText::AsNumber(MaxAmount)});
}
