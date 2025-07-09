// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"
#include "PGA_Attack_Melee_Heavy.generated.h"


class UAT_Attack_Throw_Weapon;
class UAT_Melee_Attack_Throw_Weapon;
/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack_Melee_Heavy : public UPGA_Attack_Melee
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	virtual void ThrowWeapon(FGameplayEventData Data);
	

	UPROPERTY()
	UAT_Melee_Attack_Throw_Weapon* Task;
	
};
