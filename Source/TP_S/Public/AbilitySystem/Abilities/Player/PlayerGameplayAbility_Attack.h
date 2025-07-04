// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PlayerGameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility_Attack : public UPlayerGameplayAbility
{
	GENERATED_BODY()

	UPlayerGameplayAbility_Attack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION(blueprintcallable)
	FName GetNextSection();

	UFUNCTION(BlueprintCallable)
	void CheckComboInput(FGameplayEventData TargetData);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPlayerGameplayAbility> Ability;

	uint8 CurrentCombo = 0;
	uint8 CachedCurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
	
	
};
