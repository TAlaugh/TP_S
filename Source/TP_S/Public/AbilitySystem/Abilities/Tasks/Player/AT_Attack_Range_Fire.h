// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_Attack_Range_Fire.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityTaskDelegate);

/**
 * 
 */
UCLASS()
class TP_S_API UAT_Attack_Range_Fire : public UAbilityTask
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskDelegate OnStartedTask;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskDelegate OnFinishedTask;

	static UAT_Attack_Range_Fire* Action(UGameplayAbility* OwningAbility, float Interval);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void StopFire();

protected:
	FTimerHandle Timer;
	float ActionInterval;

	void StartFire();
};
