// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Jump.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Chacracter/Player/BasePlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


void UPlayerGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetPlayerCharacterFromActorInfo();
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	if (CachedPlayerCharacter.IsValid() && CachedPlayerCharacter->CanJump())
	{
		CachedPlayerCharacter->Jump();
		if (JumpStartMontage)
		{
			CachedPlayerCharacter->PlayAnimMontage(JumpStartMontage);
		}

		WaitLand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Shared_Event_Land,
			nullptr,
			false,
			true
		);

		WaitLand->EventReceived.AddDynamic(this, &ThisClass::OnLandedEvent);
		WaitLand->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);		
	}
	
}

void UPlayerGameplayAbility_Jump::OnLandedEvent(FGameplayEventData Payload)
{
	Debug::Print("Lande2d",FColor::Green);
	if (CachedPlayerCharacter.IsValid() && JumpEndMontage)
	{
		CachedPlayerCharacter->PlayAnimMontage(JumpEndMontage);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
