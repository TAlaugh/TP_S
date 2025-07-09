// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "BasePlayerLinkedAnimLayer.generated.h"

class UBasePlayerAnimInstance;
/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerLinkedAnimLayer : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UBasePlayerAnimInstance* GetPlayerAnimInstance() const;
};
