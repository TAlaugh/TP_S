// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AT_Attack_Melee_ThrowWeapon.generated.h"

class ABasePlayerCharacter;
class ABasePlayerWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UAT_Attack_Melee_ThrowWeapon : public UAbilityTask_WaitGameplayEvent
{
	GENERATED_BODY()
public:

	static UAT_Attack_Melee_ThrowWeapon* Init(UGameplayAbility* OwningAbility, FGameplayTag EventTag, bool OnlyTriggerOnce = true, bool OnlyMatchExact = true);

	virtual void Activate() override;
	virtual void GameplayEventContainerCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload) override;
	virtual void TickTask(float DeltaTime) override;

protected:

	UPROPERTY()
	ABasePlayerCharacter* Player;
	UPROPERTY()
	ABasePlayerWeapon* Weapon;

	FVector PlayerLocation;
	FVector WeaponLocation;
	FVector TargetLocation;
	float InterpSpeed = 5.f;
	FTimerHandle Timer;
	bool bHasThrow = false;
	bool bHasArrived = false;

};
