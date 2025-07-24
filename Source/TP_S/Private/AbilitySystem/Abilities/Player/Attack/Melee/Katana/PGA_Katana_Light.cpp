// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/Katana/PGA_Katana_Light.h"

#include "Items/Weapons/BasePlayerWeapon.h"

UPGA_Katana_Light::UPGA_Katana_Light()
{
	WeaponSocketName = FName("hand_lKatanaSocket");
}

void UPGA_Katana_Light::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
