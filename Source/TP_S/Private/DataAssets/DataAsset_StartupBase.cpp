// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_StartupBase.h"

#include "DebugHelper.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

void UDataAsset_StartupBase::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 ApplyLevel)
{
	check(ASC);

	GrantAbilities(ActivateOnGivenAbilities, ASC);
	GrantAbilities(ReactiveAbilities, ASC);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
			ASC->ApplyGameplayEffectToSelf(Effect, ApplyLevel, ASC->MakeEffectContext());
		}
	}
}

void UDataAsset_StartupBase::GrantAbilities(TArray<TSubclassOf<UBaseGameplayAbility>>& InAbilitiesToGive,
	UBaseAbilitySystemComponent* ASC, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}
	
	for (const TSubclassOf<UBaseGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		const UBaseGameplayAbility* AbilityCDO = Ability.GetDefaultObject();

		
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.InputID = static_cast<int32>(AbilityCDO->AbilityInputID); 
		
		ASC->GiveAbility(AbilitySpec);
	
	}
}