// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PGA_Death.h"

#include "BaseGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/BasePlayerCharacter.h"

UPGA_Death::UPGA_Death()
{
	AbilityTags.AddTag(BaseGamePlayTags::Shared_Ability_Death);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability);
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerTag = BaseGamePlayTags::Shared_Status_Dead;
	AbilityTriggers.Add(TriggerData);
}

void UPGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (MontageToPlay)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("None"),
			MontageToPlay);
		Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
		Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Task->ReadyForActivation();
	}
}

void UPGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerCharacterFromActorInfo()->OnDead();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
