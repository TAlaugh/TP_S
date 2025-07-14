// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemDataAsset.h"
#include "BaseType/Player/PlayerStructType.h"
#include "WeaponItemDataAsset.generated.h"

class ABasePlayerWeapon;
class UGameplayAbility;
class UAnimMontage;

/**
 * 
 */

UCLASS()
class TP_S_API UWeaponItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UWeaponItemDataAsset();
	
	/** 기본 공격력 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float BaseDamage = 20.f;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ABasePlayerWeapon> WeaponClass;

	virtual void GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	UMaterialInterface* EquipWeaponMaterial;
};
