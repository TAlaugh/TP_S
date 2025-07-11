// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WeaponHUDWidget.h"

#include "Components/Image.h"
#include "Items/Weapons/WeaponItemDataAsset.h"

void UWeaponHUDWidget::UpdateWeaponDisplay(UWeaponItemDataAsset* WeaponItemData)
{
	if (!WeaponImage) return;
	
	if (WeaponItemData && WeaponItemData->EquipWeaponMaterial)
	{
		WeaponImage->SetBrushFromMaterial(WeaponItemData->EquipWeaponMaterial);
	}
	else
	{
		WeaponImage->SetBrushFromMaterial(DefaultMaterial);
	}
}
