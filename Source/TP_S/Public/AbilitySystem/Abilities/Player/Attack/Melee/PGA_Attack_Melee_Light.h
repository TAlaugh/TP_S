// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"
#include "PGA_Attack_Melee_Light.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_Light : public UPGA_Attack_Melee
{
	GENERATED_BODY()

public:
	UPGA_Attack_Melee_Light();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void HandleApplyDamage(FGameplayEventData Data) override;
	virtual FName GetNextSection() override;

	UFUNCTION()
	virtual void SetNextSection(FGameplayEventData Data) override;
	
	UFUNCTION(BlueprintCallable)
	void StopAttack(FGameplayEventData Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> MontageByTag;

	
	

	
};
