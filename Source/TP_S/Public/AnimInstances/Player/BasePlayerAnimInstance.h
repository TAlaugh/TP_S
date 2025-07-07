// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "BasePlayerAnimInstance.generated.h"

class ABasePlayerCharacter;
/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	ABasePlayerCharacter* OwningPlayerCharacter;
	
	// 왼발이 앞에 있는지, 오른발이 앞에 있는지 : 왼발 = 1, 오른발 = -1
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float FootPhase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bCrouch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bFalling;
	
};
