// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Hit/PGA_Death.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"

UPGA_Death::UPGA_Death()
{
	AbilityTags.AddTag(BaseGamePlayTags::Shared_Ability_Death);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability);
	CancelAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability);
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	TriggerData.TriggerTag = BaseGamePlayTags::Shared_Status_Dead;
	AbilityTriggers.Add(TriggerData);
}

void UPGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	MovementFix(true);
	if (MontageToPlay)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("None"),
			MontageToPlay);
		//Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
		//Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		//Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		//Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Task->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* DeathTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Shared_Event_Death);
		DeathTask->EventReceived.AddDynamic(this, &ThisClass::OnDeathEvent);
		DeathTask->ReadyForActivation();
	}
}

void UPGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGA_Death::OnDeathEvent(FGameplayEventData Data)
{
	GetPlayerCharacterFromActorInfo()->OnDead();
}
