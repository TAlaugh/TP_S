// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PGA_Spawn_Weapon.generated.h"

class ABasePlayerWeapon;
class ABaseWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Spawn_Weapon : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPGA_Spawn_Weapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName SocketNameToAttachment;

	UPROPERTY(EditDefaultsOnly,	BlueprintReadOnly, Category= "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ABasePlayerWeapon* Weapon;

	

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
