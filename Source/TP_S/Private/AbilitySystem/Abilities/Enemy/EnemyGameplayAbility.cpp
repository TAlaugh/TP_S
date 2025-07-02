// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/EnemyGameplayAbility.h"
#include "BaseGamePlayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
//#include "Chacracter/Enemy/BaseEnemyCharacter.h"
#include "TP_S/Public/Character/Enemy/BaseEnemyCharacter.h"

ABaseEnemyCharacter* UEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<ABaseEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}


UEnemyCombatComponent* UEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> Effect,
	const FScalableFloat& DamageFloat)
{
	check(Effect);

	FGameplayEffectContextHandle ContextHandle;
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
	Effect,
	GetAbilityLevel(),
	ContextHandle
	);

	SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_BaseDamage,
		DamageFloat.GetValueAtLevel(GetAbilityLevel()));
	
	return SpecHandle;
}
