// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PGA_Attack.generated.h"

class ABasePlayerWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UPGA_Attack : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPGA_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	virtual void HandleApplyDamage(FGameplayEventData Data);
	
	UFUNCTION()
	virtual void EquipWeapon();
	UFUNCTION()
	virtual void UnEquipWeapon();

	UFUNCTION()
	virtual void EquipWeaponFromEvent(FGameplayEventData Data);
	UFUNCTION()
	virtual void UnEquipWeaponFromEvent(FGameplayEventData Data);

protected:	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPlayerGameplayAbility> Ability;

	UPROPERTY()
	UBasePlayerCombatComponent* PlayerCombatComponent;

	UPROPERTY()
	FName WeaponSocketName;

	UPROPERTY()
	FGameplayTag WeaponType;

	UPROPERTY()
	FGameplayTag AttackType;

	UPROPERTY()
	ABasePlayerWeapon* Weapon;

	UPROPERTY()
	bool bUnEquipWhenEnd = true;
};
