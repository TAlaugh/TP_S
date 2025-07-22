// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Hit/PGA_HitReact.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"

UPGA_HitReact::UPGA_HitReact()
{
	AbilityTags.AddTag(BaseGamePlayTags::Shared_Ability_HitReact);
	CancelAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability);
}

void UPGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), BaseGamePlayTags::Shared_Status_InAir))
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_InAir];
	}
	else
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Idle];
	}
}


void UPGA_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
