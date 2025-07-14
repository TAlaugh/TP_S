// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/PGA_Attack.h"
#include "PGA_Attack_Range.generated.h"

class UAT_Attack_Range_Fire;
/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Range : public UPGA_Attack
{
	GENERATED_BODY()
public:
	UPGA_Attack_Range();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void HandleApplyDamage(FGameplayEventData Data) override;
	UPROPERTY()
	UAT_Attack_Range_Fire* FireTask;
	
	TWeakObjectPtr<AActor>* Bullet;
	
	float BulletCount;
	float MaxBulletCount;

	UFUNCTION()
	void HandleFire();

	UFUNCTION()
	void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimLayer;
};
