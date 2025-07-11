// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PGA_Movement.h"
#include "PlayerGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UPlayerGameplayAbility_Jump : public UPGA_Movement
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void Jump();

	UFUNCTION()
	void SecondJump();

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
