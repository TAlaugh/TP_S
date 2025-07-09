// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UGEEC_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UGEEC_DamageTaken();

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
