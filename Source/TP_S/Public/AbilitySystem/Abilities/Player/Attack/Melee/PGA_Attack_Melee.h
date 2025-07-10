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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	void HandleApplyDamage(FGameplayEventData Data);

	virtual void EquipWeapon(FName SocketName = FName("hand_rSocket")) override;
	virtual void UnEquipWeapon(FGameplayEventData TargetData) override;

	UFUNCTION()
	virtual void EquipWeaponLeftSocket(FGameplayEventData Data) override;

	UFUNCTION()
	virtual void EquipWeaponRightSocket(FGameplayEventData Data) override;

	FTimerHandle TimerHandle;
};
