// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Attack.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"

UPlayerGameplayAbility_Attack::UPlayerGameplayAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UPlayerGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("Attack"),
		MontageToPlay,
		1.f,
		GetNextSection(),
		false);
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* TaskToNext = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Ability_Attack_Melee_Light
		);
	TaskToNext->EventReceived.AddDynamic(this, &UPlayerGameplayAbility_Attack::CheckComboInput);
	TaskToNext->ReadyForActivation();
	
	
}

void UPlayerGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	HasNextComboInput = true;
}

void UPlayerGameplayAbility_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPlayerGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentCombo = 0;
	HasNextComboInput = false;
}

void UPlayerGameplayAbility_Attack::OnCompleteCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UPlayerGameplayAbility_Attack::GetNextSection()
{
	CurrentCombo++;
	if (CurrentCombo > 5)
	{
		CurrentCombo = 1;
	}
	return *FString::Printf(TEXT("%d"), CurrentCombo);
}


void UPlayerGameplayAbility_Attack::CheckComboInput(FGameplayEventData TargetData)
{
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		HasNextComboInput = false;
	}
	else
	{
		OnCompleteCallback();
	}
}
