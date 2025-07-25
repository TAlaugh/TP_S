// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class ABasePlayerCharacter;
class ABasePlayerController;
class UBasePlayerCombatComponent;

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerGameplayAbility();
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	ABasePlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	ABasePlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	UBasePlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakePlayerDamageGameplayEffectHandle(TSubclassOf<UGameplayEffect> Effect, float WeaponBaseDamage, FGameplayTag AttackTypeTag, int32 ComboCount);

protected:
	TWeakObjectPtr<ABasePlayerCharacter> CachedPlayerCharacter;
	TWeakObjectPtr<ABasePlayerController> CachedPlayerController;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual FName GetNextSection();
	virtual void SetNextSection(FGameplayEventData Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
	uint8 MaxSection = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Section")
	uint8 CurrentSection = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* MontageToPlay;

	// End, Cancel 콜백
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	// bCan = true -> 움직임가능, bCan = false -> 움직임불가능
	UFUNCTION()
	void MovementFix(bool bCan);

	// bCan = true -> 회전 가능, bCan = false -> 회전 불가능
	UFUNCTION()
	void DirectionFix(bool bCan);

	UFUNCTION()
	void RotationSetByAim(bool bCan);

	EPlayerState CurrentPlayerState = EPlayerState::None;
};
