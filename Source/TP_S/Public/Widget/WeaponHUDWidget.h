// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponHUDWidget.generated.h"

class UImage;
class UWeaponItemDataAsset;
/**
 * 
 */
UCLASS()
class TP_S_API UWeaponHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateWeaponDisplay(UMaterialInterface* NewMaterial);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UMaterialInterface* DefaultMaterial;
};
