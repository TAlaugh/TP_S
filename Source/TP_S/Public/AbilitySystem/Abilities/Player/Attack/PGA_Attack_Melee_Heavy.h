// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Attack.h"

#include "PGA_Attack_Melee_Heavy.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_Heavy : public UPlayerGameplayAbility_Attack
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void EquipWeapon() override;
	
	virtual void UnEquipWeapon(FGameplayEventData TargetData) override;

	UFUNCTION()
	virtual void ThrowWeapon(FGameplayEventData Data);

	
};
