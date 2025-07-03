// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Attack.h"

#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), MontageToPlay, 1.f, GetNextSection());
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
	
	Task->ReadyForActivation();
	Debug::Print("Activate");
}

void UPlayerGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (!ComboTimer.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}

	Debug::Print("InputPressed");
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
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
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

void UPlayerGameplayAbility_Attack::StartComboTimer()
{
	const float FrameRate = 1.5f;
	//GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &ThisClass::CheckComboInput, FrameRate,false);
}

void UPlayerGameplayAbility_Attack::CheckComboInput()
{
	ComboTimer.Invalidate();

	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
