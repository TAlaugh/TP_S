// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class ABasePlayerCharacter;
class ABasePlayerController;
class UBasePlayerCombatComponent;

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	ABasePlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	ABasePlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	UBasePlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakePlayerDamageGameplayEffectHandle(TSubclassOf<UGameplayEffect> Effect, float WeaponBaseDamage, FGameplayTag AttackTypeTag, int32 ComboCount);

protected:
	TWeakObjectPtr<ABasePlayerCharacter> CachedPlayerCharacter;
	TWeakObjectPtr<ABasePlayerController> CachedPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* MontageToPlay;

	// 디버깅용
	/*
	UFUNCTION()
	virtual	void OnCanceled() const;

	UFUNCTION()
	virtual void OnCompleted() const;

	UFUNCTION()
	virtual void OnInterrupted() const;

	UFUNCTION()
	virtual void OnBlendOut() const;
	*/
};
