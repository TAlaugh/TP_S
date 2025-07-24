// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PoleArm/PGA_PoleArm_Fall.h"

UPGA_PoleArm_Fall::UPGA_PoleArm_Fall()
{
	WeaponSocketName = FName("hand_rPoleArmSocket");
}

void UPGA_PoleArm_Fall::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
