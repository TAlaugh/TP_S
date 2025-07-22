// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PGA_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_HitReact : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPGA_HitReact();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> MontageByTag;
	
};
