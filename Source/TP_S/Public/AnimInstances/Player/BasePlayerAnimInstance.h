// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "BasePlayerAnimInstance.generated.h"

class UBaseAbilitySystemComponent;
class ABasePlayerCharacter;
/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UBasePlayerAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UBaseAbilitySystemComponent* ASC);
protected:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	ABasePlayerCharacter* OwningPlayerCharacter;
	
	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
	
	// 왼발이 앞에 있는지, 오른발이 앞에 있는지 : 왼발 = 1, 오른발 = -1
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float FootPhase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bCrouch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bFalling;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float CachedLocomotionDirection;

	
};
