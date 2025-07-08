// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Attack.h"
#include "PGA_Attack_Melee_Light.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_Light : public UPlayerGameplayAbility_Attack
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	virtual FName GetNextSection() override;

	UFUNCTION(BlueprintCallable)
	void NextCombo(FGameplayEventData TargetData);
	
	virtual void EquipWeapon(FName SocketName = FName("hand_rSocket")) override;
	
	virtual void UnEquipWeapon(FGameplayEventData TargetData) override;

	UFUNCTION()
	virtual void EquipWeaponLeftSocket(FGameplayEventData Data) override;

	UFUNCTION()
	virtual void EquipWeaponRightSocket(FGameplayEventData Data) override;

	uint8 CurrentCombo = 0;
	uint8 CachedCurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
	FTimerHandle TimerHandle;
};
