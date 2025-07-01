// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "BaseGamePlayTags.h"
#include "DebugHelper.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "BaseType/BaseStructType.h"

void UBaseAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		TryActivateAbility(Spec.Handle);
	}
}

void UBaseAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(BaseGamePlayTags::InputTag_Hold) )
	{
		return;
	}
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
		{
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitiySet>& WeaponAbilities,
	int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (WeaponAbilities.IsEmpty())
	{
		return;
	}
	// OutGrantedAbilitySpecHandles : 어빌리티를 제어(실행,중지,간섭)할 수 있는 Handle 배열
	// 이 배열에 Spec(어빌리티 내용)을 추가 -> .AddUnique
	// GiveAbility(Spec) : Spec을 추가하고 Spec을 제어할 Handle 반환
	// GiveAbilityAndActivateOnce(Spec) : Spec은 반드시 인스턴스(실행)되어야 하며, 인스턴스화 성공했을 시 Handle 반환, 실패했을 시는 Spec자체가 부여되지 않음.
	for (const FPlayerAbilitiySet& WeaponAbilitySet : WeaponAbilities)
	{
		if (!WeaponAbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(WeaponAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(WeaponAbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(Spec));
	}
}

void UBaseAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{
	if (SpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}

bool UBaseAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpecs;

	// 활성화 가능한 모든 게임어빌리티 스펙을 가져온다
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), AbilitySpecs);

	// AbilitySpecs 배열에 받은 데이터가 유효하면
	if (!AbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, AbilitySpecs.Num() - 1);

		FGameplayAbilitySpec* AbilitySpec = AbilitySpecs[RandomAbilityIndex];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}
	}

	return false;
}
