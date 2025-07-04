// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "BaseGameplayAbility.generated.h"

class UBaseAbilitySystemComponent;
class UBaseCombatComponent;

UENUM()
enum class EbaseAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class TP_S_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	UBaseCombatComponent* GetBaseCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponentFromActorInfo() const;
	
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle);
	
protected:
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "BaseAbility")
	EbaseAbilityActivationPolicy AbilityActivationPolicy = EbaseAbilityActivationPolicy::OnTriggered;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseAbility")
	EAbility AbilityInputID = EAbility::None;
	
};
