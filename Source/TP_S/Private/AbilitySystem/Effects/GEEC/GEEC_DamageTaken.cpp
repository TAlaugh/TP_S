// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GEEC/GEEC_DamageTaken.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAttributeSet.h"

struct FDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FDamageCapture()
	{
		// Source GE -> 주체 , Target GE -> 대상
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DamageTaken, Target, false);
	}
};

static const FDamageCapture& GetDamageCapture()
{
	static FDamageCapture DamageCapture;
	return DamageCapture;
}

UGEEC_DamageTaken::UGEEC_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().AttackDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().DamageTakenDef);
}

void UGEEC_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().AttackDef, EvaluateParams, SourceAttack);

	float BaseDamage = 0.f;
	int32 CachedComboCount_Light = 0;
	int32 CachedComboCount_Heavy = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : Spec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key == BaseGamePlayTags::Shared_SetByCaller_BaseDamage)
		{
			BaseDamage = TagMagnitude.Value;
		}
		// 콤보 구현
	}

	
	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().DefenseDef, EvaluateParams, TargetDefense);

	if (CachedComboCount_Light > 0)
	{
		const float ComboDamage = 0.f;
		BaseDamage *= ComboDamage;
	}
	
	if (CachedComboCount_Heavy > 0)
	{
		const float ComboDamage = 0.f;
		BaseDamage *= ComboDamage;
	}

	const float FinalDamage = BaseDamage * SourceAttack / TargetDefense;

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			GetDamageCapture().DamageTakenProperty,
			EGameplayModOp::Override,
			FinalDamage));
	}
	
}
