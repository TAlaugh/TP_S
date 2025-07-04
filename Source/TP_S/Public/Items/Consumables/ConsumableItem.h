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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmount = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CooldownTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> UseAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTypeTag;
};
