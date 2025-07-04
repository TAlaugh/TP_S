// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Player/DataAsset_StartupBasePlayer.h"

#include "DebugHelper.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "AbilitySystem/Player/PlayerAbilitySystemComponent.h"
#include "BaseType/BaseStructType.h"

void UDataAsset_StartupBasePlayer::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(ASC, ApplyLevel);

	for (const FPlayerAbilitiySet& AbilitySet : PlayerStartupAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(AbilitySet.AbilityToGrant);
		Spec.SourceObject = ASC->GetAvatarActor();
		Spec.Level = ApplyLevel;
		Spec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		Spec.InputID = static_cast<int32>(AbilitySet.InputId);
		ASC->GiveAbility(Spec);
	}
}
