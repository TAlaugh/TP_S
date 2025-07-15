// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_ReceiveWeapon.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ReceiveWeapon.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

UPGA_Attack_Melee_ReceiveWeapon::UPGA_Attack_Melee_ReceiveWeapon()
{
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_ReceiveWeapon);
	ActivationRequiredTags.AddTag(BaseGamePlayTags::Player_Status_WeaponThrown);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	AbilityInputID = EAbility::None;
}

void UPGA_Attack_Melee_ReceiveWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	PlayerCombatComponent = GetPlayerCombatComponentFromActorInfo();
	bUnEquipWhenEnd = true;
	if (GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->CurrentThrownWeaponTag.IsValid())
	{
		UAbilityTask_PlayMontageAndWait* ReceiveTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"ReceiveWeapon",
			MontageToPlay,
			1.f,
			FName("Default"),
			false);
		ReceiveTask->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
		ReceiveTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		ReceiveTask->OnInterrupted.RemoveDynamic(this, &ThisClass::OnCompleteCallback);
		ReceiveTask->OnCancelled.RemoveDynamic(this, &ThisClass::OnCompleteCallback);
		ReceiveTask->ReadyForActivation();
		
		UAT_Attack_Melee_ReceiveWeapon* Task = UAT_Attack_Melee_ReceiveWeapon::Init(this);
		Task->OnReceivedDelegate.AddDynamic(this, &ThisClass::OnCompleteCallback);
		Task->ReadyForActivation();
	} else
	{
		K2_EndAbility();
	}
}
