// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/PGA_Attack_Melee_Heavy.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Melee_Attack_Throw_Weapon.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UPGA_Attack_Melee_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	EquipWeapon();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ThrowWeapon(FGameplayEventData());	
}

void UPGA_Attack_Melee_Heavy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnEquipWeapon(FGameplayEventData());
}

void UPGA_Attack_Melee_Heavy::EquipWeapon(FName SocketName)
{
	GetPlayerCombatComponentFromActorInfo()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
}

void UPGA_Attack_Melee_Heavy::UnEquipWeapon(FGameplayEventData TargetData)
{
	GetPlayerCombatComponentFromActorInfo()->UnEquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
}

void UPGA_Attack_Melee_Heavy::ThrowWeapon(FGameplayEventData Data)
{
	ABasePlayerWeapon* Weapon = CachedPlayerCharacter->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	if (Weapon)
	{
		Task = UAT_Melee_Attack_Throw_Weapon::Init(this, BaseGamePlayTags::Player_Event_Attack_Throw);
		Task->ReadyForActivation();
	}
}

