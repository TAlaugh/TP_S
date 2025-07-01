// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Enemy/DataAsset_StartupBaseEnemy.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Enemy/EnemyGameplayAbility.h"


void UDataAsset_StartupBaseEnemy::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level)
{
	Super::GiveToAbilitySystemComponent(ASC, Level);

	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = ASC->GetAvatarActor();
			AbilitySpec.Level = Level;
			ASC->GiveAbility(AbilitySpec);
		}
	}
}
