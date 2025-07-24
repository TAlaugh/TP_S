// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PoleArm/PGA_PoleArm_Light.h"

UPGA_PoleArm_Light::UPGA_PoleArm_Light()
{
	WeaponSocketName = FName("hand_rPoleArmSocket");
}

void UPGA_PoleArm_Light::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
