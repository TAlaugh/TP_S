// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"
#include "Controllers/BasePlayerController.h"


UPlayerGameplayAbility::UPlayerGameplayAbility()
{
	//GetPlayerControllerFromActorInfo();
}

ABasePlayerCharacter* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<ABasePlayerCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedPlayerCharacter.IsValid() ? CachedPlayerCharacter.Get() : nullptr;
}

ABasePlayerController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<ABasePlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedPlayerController.IsValid() ? CachedPlayerController.Get() : nullptr;
}

UBasePlayerCombatComponent* UPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerDamageGameplayEffectHandle(
	TSubclassOf<UGameplayEffect> Effect, float WeaponBaseDamage, FGameplayTag AttackTypeTag, int32 ComboCount)
{
	//check(Effect);
	if (Effect)
	{
		FGameplayEffectContextHandle ContextHandle;
		ContextHandle.SetAbility(this);
		ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
		ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

		FGameplayEffectSpecHandle SpecHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(Effect, GetAbilityLevel(), ContextHandle);
		SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_BaseDamage, WeaponBaseDamage);

		if (AttackTypeTag.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(AttackTypeTag, ComboCount);
		}

		return SpecHandle;
	}
	return nullptr;
}

void UPlayerGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UPlayerGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}
