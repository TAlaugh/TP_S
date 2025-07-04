// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"

class ABaseEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class TP_S_API UEnemyGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Ability")
	ABaseEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Abililty")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Abililty")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect, const FScalableFloat& DamageFloat);

private:
	TWeakObjectPtr<ABaseEnemyCharacter> CachedEnemyCharacter;
};
