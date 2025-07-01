// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PlayerGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility_Jump : public UPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	virtual void OnLandedEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnCanceled();

	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnInterrupted();

	UFUNCTION()
	void OnBlendOut();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JumpStartMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JumpEndMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JumpLoopMontage;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitLand;

	/*
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait;
	*/

	UPROPERTY(EditAnywhere, Category = "Data")
	float JumpX;
	
	UPROPERTY(EditAnywhere, Category = "Data")
	float JumpY;

	UPROPERTY(EditAnywhere, Category = "Data")
	float JumpZ;

	UPROPERTY(EditAnywhere, Category = "Data")
	float DashX;

	UPROPERTY(EditAnywhere, Category = "Data")
	float DashY;

	UPROPERTY(EditAnywhere, Category = "Data")
	float DashZ;
};
