#pragma once

#include "GameplayTagContainer.h"
#include "PlayerEnumType.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PlayerStructType.generated.h"

class UPlayerGameplayAbility;

USTRUCT(BlueprintType)
struct FPlayerAbilitiySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Catgories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerGameplayAbility> AbilityToGrant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAbility InputId = EAbility::None;

	bool IsValid() const
	{
		return InputTag.IsValid() && AbilityToGrant;
	};
	
};
USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHeroLinkedAnimLayer> WeaponAnimLayerLink;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;
	*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerAbilitiySet> WeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
};