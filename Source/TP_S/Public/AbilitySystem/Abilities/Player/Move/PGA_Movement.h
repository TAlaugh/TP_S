// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "PGA_Movement.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Movement : public UPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> MontageByTag;

	virtual void Action();
};
