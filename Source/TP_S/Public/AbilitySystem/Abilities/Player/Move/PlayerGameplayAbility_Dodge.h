// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PGA_Movement.h"
#include "PlayerGameplayAbility_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility_Dodge : public UPGA_Movement
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> MontageInAirByTag;

	UPROPERTY()
	TMap<FGameplayTag, UAnimMontage*> MontageInGroundByTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> MontageEndByTag;
	
	UAnimMontage* MontageEnd;
	
};
