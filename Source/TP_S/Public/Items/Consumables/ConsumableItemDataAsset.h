// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemDataAsset.h"
#include "ConsumableItemDataAsset.generated.h"

class UGameplayAbility;

/**
 * 
 */
UCLASS()
class TP_S_API UConsumableItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UConsumableItemDataAsset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Consumable|Heal")
	float HpPercent = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Consumable|Heal")
	float HpFloat = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Consumable|Heal")
	float AtkFloat = 0.f;

	virtual void GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const override;
};
