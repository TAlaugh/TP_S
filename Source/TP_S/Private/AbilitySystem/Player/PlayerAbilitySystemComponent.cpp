// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Player/PlayerAbilitySystemComponent.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
void UPlayerAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		if (Spec.InputID != static_cast<int32>(EAbility::Attack))
		{
			AbilitySpecInputPressed(Spec);
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UPlayerAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(BaseGamePlayTags::InputTag_Hold) )
	{
		return;
	}
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
		{
			if (Spec.InputID != static_cast<int32>(EAbility::Attack))
			{
				AbilitySpecInputReleased(Spec);
				CancelAbilityHandle(Spec.Handle);
			}
		}
	}
}

void UPlayerAbilitySystemComponent::OnAbilityInputTriggered(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		if (Spec.InputID == static_cast<int32>(EAbility::Attack))
		{
			AbilitySpecInputPressed(Spec);
			TryActivateAbility(Spec.Handle);
		}
	}
}
