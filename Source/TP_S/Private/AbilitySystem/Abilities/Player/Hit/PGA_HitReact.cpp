// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Hit/PGA_HitReact.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DebugHelper.h"

UPGA_HitReact::UPGA_HitReact()
{
	AbilityTags.AddTag(BaseGamePlayTags::Shared_Ability_HitReact);
	CancelAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement_Jump);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement_Crouch);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement_Slide);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Shared_Ability_Death);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Shared_Status_Dead);
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerTag = BaseGamePlayTags::Shared_Event_HitReact;
	AbilityTriggers.Add(TriggerData);
}

void UPGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	MovementFix(true);
	const AActor* Attacker = TriggerEventData->Instigator.Get();

	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), BaseGamePlayTags::Shared_Status_InAir))
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_InAir];
		CurrentPlayerState = EPlayerState::InAir;
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), Attacker->GetActorLocation());
		GetPlayerCharacterFromActorInfo()->SetActorRotation(Rotation);

		UAbilityTask_WaitGameplayEvent* LandTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Shared_Event_Land);
		LandTask->EventReceived.AddDynamic(this, &ThisClass::SetNextSection);
		LandTask->ReadyForActivation();
	}
	else
	{
		FGameplayTag DirectionTag = UBaseFunctionLibrary::NativeComputeHitReactDirectionTag(Attacker, GetAvatarActorFromActorInfo());
		if (MontageByTag.Contains(DirectionTag))
		{
			MontageToPlay = MontageByTag[DirectionTag];
		}
	}

	if (MontageToPlay)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("None"),
			MontageToPlay,
			1,
			GetNextSection());
		Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
		Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Task->ReadyForActivation();
	}
}


void UPGA_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	MovementFix(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UPGA_HitReact::GetNextSection()
{
	CurrentSection++;
	if (CurrentSection > MaxSection)
	{
		CurrentSection = 1;
	}
	return *FString::Printf(TEXT("%d"), CurrentSection);
}

void UPGA_HitReact::SetNextSection(FGameplayEventData Data)
{
	MontageJumpToSection(GetNextSection());
}
