// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GE/GE_DealDamage.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Effects/GEEC/GEEC_DamageTaken.h"

UGE_DealDamage::UGE_DealDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UBaseAttributeSet::GetAttackAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Override;
	ModifierInfo.ModifierMagnitude = FScalableFloat(0.0f);
	//Modifiers.Add(ModifierInfo);
	
	FGameplayEffectExecutionDefinition GameplayEffectExecution;
	GameplayEffectExecution.CalculationClass = UGEEC_DamageTaken::StaticClass();
	Executions.Add(GameplayEffectExecution);
}
