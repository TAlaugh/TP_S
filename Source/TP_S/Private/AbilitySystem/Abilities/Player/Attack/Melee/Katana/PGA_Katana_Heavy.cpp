// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/Katana/PGA_Katana_Heavy.h"

#include "BaseGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UPGA_Katana_Heavy::UPGA_Katana_Heavy()
{
	WeaponSocketName = FName("hand_lKatanaSocket");
}

void UPGA_Katana_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Finish);
	WaitTask->EventReceived.AddDynamic(this, &ThisClass::StopAttack);
	WaitTask->ReadyForActivation();
}
