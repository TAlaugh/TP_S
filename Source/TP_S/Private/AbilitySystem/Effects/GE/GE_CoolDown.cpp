// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GE/GE_CoolDown.h"

#include "BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UGE_CoolDown::UGE_CoolDown(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}
