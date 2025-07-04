// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseType/BaseStructType.h"

#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"

bool FPlayerAbilitiySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
