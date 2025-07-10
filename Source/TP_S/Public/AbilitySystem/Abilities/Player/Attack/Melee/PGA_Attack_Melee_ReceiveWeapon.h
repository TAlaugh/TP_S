// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"
#include "PGA_Attack_Melee_ReceiveWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_ReceiveWeapon : public UPGA_Attack_Melee
{
	GENERATED_BODY()
public:
	UPGA_Attack_Melee_ReceiveWeapon();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
