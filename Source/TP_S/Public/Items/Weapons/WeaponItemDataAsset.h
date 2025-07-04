// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemDataAsset.h"
#include "WeaponItemDataAsset.generated.h"

class UGameplayAbility;
class UAnimMontage;

/**
 * 
 */

UCLASS()
class TP_S_API UWeaponItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UWeaponItemDataAsset();
	
	/** 기본 공격력 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float BaseDamage = 20.f;

	/** 공격 애니메이션 몽타주(Soft 로더) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSoftObjectPtr<UAnimMontage> AttackMontage;
};
