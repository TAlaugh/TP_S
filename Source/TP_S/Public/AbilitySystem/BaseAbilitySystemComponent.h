// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseType/BaseStructType.h"
#include "GameplayTagContainer.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);
	
	// Grant Abilities in WeaponData
	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (Level = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitiySet>& WeaponAbilities, int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	// Remove Granted Spec Handle Abililites in WeaponData
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTag(FGameplayTag Tag);
};
