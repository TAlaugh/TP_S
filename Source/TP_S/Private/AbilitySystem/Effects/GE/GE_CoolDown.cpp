// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GE/GE_CoolDown.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UGE_CoolDown::UGE_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(5.f));
	//UTargetTagsGameplayEffectComponent* Tags = NewObject<UTargetTagsGameplayEffectComponent>();
	//Tags
	//GEComponents.Add(Tags);
}
