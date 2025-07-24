// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PoleArm/PGA_PoleArm_Heavy.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ThrowWeapon.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

UPGA_PoleArm_Heavy::UPGA_PoleArm_Heavy()
{
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Status_WeaponThrown);
	WeaponSocketName = FName("hand_rPoleArmSocket");
}

void UPGA_PoleArm_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ThrowWeapon(FGameplayEventData());
}

void UPGA_PoleArm_Heavy::ThrowWeapon(FGameplayEventData Data)
{
	Super::ThrowWeapon(Data);
	Weapon = CachedPlayerCharacter->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	if (Weapon)
	{
		Task = UAT_Attack_Melee_ThrowWeapon::Init(this, BaseGamePlayTags::Player_Event_Attack_Throw);
		Task->ReadyForActivation();
		bUnEquipWhenEnd = false;
	}
}
