// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

struct FGameplayTag;
class UItemDataAsset;

USTRUCT(BlueprintType)
struct FStoredItem
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY()
	int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FStoredQuickSlot
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY()
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FStoredCombatInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag MeleeWeaponTag;

	UPROPERTY()
	FGameplayTag RangeWeaponTag;
};

USTRUCT(BlueprintType)
struct FStoredAttributeDate
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentHp = 0.f;

	UPROPERTY()
	float MaxHp = 0.f;
};

/**
 * 
 */
UCLASS()
class TP_S_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FStoredItem> StoredInventory;

	UPROPERTY()
	FStoredQuickSlot StoredQuickSlot;

	UPROPERTY()
	FStoredCombatInfo StoredCombatInfo;

	UPROPERTY()
	FStoredAttributeDate StoredAttributes;
	
	void CaptureFromComponents(ACharacter* PlayerChar);
	void RestoreToComponents(ACharacter* PlayerChar);
	
};
