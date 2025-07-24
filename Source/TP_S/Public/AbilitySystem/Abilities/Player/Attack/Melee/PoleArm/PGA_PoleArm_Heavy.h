// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Heavy.h"
#include "PGA_PoleArm_Heavy.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_PoleArm_Heavy : public UPGA_Attack_Melee_Heavy
{
	GENERATED_BODY()
public:
	UPGA_PoleArm_Heavy();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ThrowWeapon(FGameplayEventData Data) override;
	
};
