// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerAnimInstance.h"
#include "BasePlayerLinkedAnimLayer.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerLinkedAnimLayer : public UBasePlayerAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UBasePlayerAnimInstance* GetPlayerAnimInstance() const;
};
