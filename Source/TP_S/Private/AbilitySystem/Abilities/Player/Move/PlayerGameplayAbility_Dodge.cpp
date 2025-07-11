// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Move/PlayerGameplayAbility_Dodge.h"



void UPlayerGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_InAir))
	{
		if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Run))
		{
			MontageToPlay = MontageInAirByTag[BaseGamePlayTags::Shared_Status_Run];
		}
		else
		{
			MontageToPlay = MontageInAirByTag[BaseGamePlayTags::Shared_Status_Idle];
		}
	}
	else
	{
		MontageInGroundByTag = MontageByTag;
		if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Run))
		{
			MontageToPlay = MontageInGroundByTag[BaseGamePlayTags::Shared_Status_Run];
		}
		else
		{
			MontageToPlay = MontageInGroundByTag[BaseGamePlayTags::Shared_Status_Idle];
		}
	}

	Action();
}

void UPlayerGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Run))
	{
		//MontageEnd = MontageEndByTag[BaseGamePlayTags::Shared_Status_Run];
	}
	else
	{
		//MontageEnd = MontageEndByTag[BaseGamePlayTags::Shared_Status_Idle];
	}
	
}

