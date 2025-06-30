// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */

class UGameplayEffect;
class UGameplayAbility;

UCLASS()
class TP_S_API UConsumableItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Icon;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmount = 10;
	
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentAmount = 1;
	
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> UseAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTypeTag;
};
