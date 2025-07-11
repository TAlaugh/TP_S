// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WeaponHUDWidget.h"

#include "Components/Image.h"
#include "Items/Weapons/WeaponItemDataAsset.h"

void UWeaponHUDWidget::UpdateWeaponDisplay(UMaterialInterface* NewMaterial)
{
	if (WeaponImage && NewMaterial)
	{
		WeaponImage->SetBrushFromMaterial(NewMaterial);
	}
}
