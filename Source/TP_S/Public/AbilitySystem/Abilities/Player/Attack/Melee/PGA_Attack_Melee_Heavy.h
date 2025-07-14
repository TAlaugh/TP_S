// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"
#include "PGA_Attack_Melee_Heavy.generated.h"


class ABasePlayerWeapon;
class UAT_Attack_Throw_Weapon;
class UAT_Attack_Melee_ThrowWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_Heavy : public UPGA_Attack_Melee
{
	GENERATED_BODY()

public:
	UPGA_Attack_Melee_Heavy();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	virtual void ThrowWeapon(FGameplayEventData Data);
	
	UPROPERTY()
	UAT_Attack_Melee_ThrowWeapon* Task;
	
	FTimerHandle ReceiveTimer;
};
