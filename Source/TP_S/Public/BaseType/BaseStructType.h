// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "BaseStructType.generated.h"

class UPlayerGameplayAbility;

USTRUCT(BlueprintType)
struct FPlayerAbilitiySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Catgories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
	
};

USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHeroLinkedAnimLayer> WeaponAnimLayerLink;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;
	*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerAbilitiySet> WeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
};