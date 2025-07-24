// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Heavy.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ThrowWeapon.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

UPGA_Attack_Melee_Heavy::UPGA_Attack_Melee_Heavy()
{
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_Heavy);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement);
	//ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Shared_Status_InAir);
	AttackType = BaseGamePlayTags::Player_Ability_Attack_Melee_Heavy;
}

void UPGA_Attack_Melee_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	MovementFix(true);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	EquipWeapon();
}


void UPGA_Attack_Melee_Heavy::ThrowWeapon(FGameplayEventData Data)
{
}

void UPGA_Attack_Melee_Heavy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	MovementFix(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

