// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/BaseWeapon.h"
#include "BasePlayerWeapon.generated.h"

class UWeaponItemDataAsset;
class UPlayerWeaponDataAsset;
/**
 * 
 */
UCLASS()
class TP_S_API ABasePlayerWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWeaponItemDataAsset> ItemDataAsset;
};
