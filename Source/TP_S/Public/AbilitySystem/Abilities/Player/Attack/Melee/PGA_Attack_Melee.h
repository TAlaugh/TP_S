// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/PGA_Attack.h"
#include "PGA_Attack_Melee.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee : public UPGA_Attack
{
	GENERATED_BODY()
public:
	UPGA_Attack_Melee();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	virtual void HandleApplyDamage(FGameplayEventData Data) override;

	virtual void UnEquipWeaponFromEvent(FGameplayEventData Data) override;
	UFUNCTION()
	virtual void EquipWeaponLeftFromEvent(FGameplayEventData Data);
	UFUNCTION()
	virtual void EquipWeaponRightFromEvent(FGameplayEventData Data);
	virtual void SetNextSection(FGameplayEventData Data) override;
	
	UFUNCTION(BlueprintCallable)
	void StopAttack(FGameplayEventData Data);

	bool HasNextComboInput = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bStopWhenAbilityEnd = false;
};
