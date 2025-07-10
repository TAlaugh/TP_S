// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AT_Attack_Melee_ReceiveWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReceivedDelegate);

class ABasePlayerCharacter;
class ABasePlayerWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UAT_Attack_Melee_ReceiveWeapon : public UAbilityTask_WaitGameplayEvent
{
	GENERATED_BODY()
public:
	static UAT_Attack_Melee_ReceiveWeapon* Init(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FReceivedDelegate OnReceivedDelegate;
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY()
	ABasePlayerCharacter* Player;
	UPROPERTY()
	ABasePlayerWeapon* Weapon;
	
	FVector PlayerLocation;
	FRotator PlayerRotation;
	FVector WeaponLocation;
	float InterpSpeed = 5.f;
};
