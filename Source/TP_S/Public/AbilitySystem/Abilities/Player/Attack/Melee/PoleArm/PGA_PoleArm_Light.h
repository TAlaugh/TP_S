// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Light.h"
#include "PGA_PoleArm_Light.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_PoleArm_Light : public UPGA_Attack_Melee_Light
{
	GENERATED_BODY()
public:
	UPGA_PoleArm_Light();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
