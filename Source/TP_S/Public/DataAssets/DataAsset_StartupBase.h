// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartupBase.generated.h"

class UGameplayEffect;
class UBaseAbilitySystemComponent;
class UBaseGameplayAbility;
/**
 * 
 */
UCLASS()
class TP_S_API UDataAsset_StartupBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 ApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(TArray<TSubclassOf<UBaseGameplayAbility>>& InAbilitiesToGive, UBaseAbilitySystemComponent* ASC, int32 ApplyLevel = 1);
};
