// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class ABasePlayerWeapon;
class ABasePlayerCharacter;
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
	TSubclassOf<ABasePlayerWeapon> MeleeWeaponClass;

	UPROPERTY()
	FGameplayTag RangeWeaponTag;

	UPROPERTY()
	TSubclassOf<ABasePlayerWeapon> RangeWeaponClass;
};

USTRUCT(BlueprintType)
struct FStoredAttributeDate
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentHp = 0.f;

	UPROPERTY()
	float MaxHp = 0.f;

	UPROPERTY()
	float Attack = 0.f;

	UPROPERTY()
	float Defence = 0.f;
};

/**
 * 
 */
UCLASS()
class TP_S_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

	ABasePlayerState();
	
public:
	UPROPERTY(Replicated)
	TArray<FStoredItem> StoredInventory;

	UPROPERTY(Replicated)
	FStoredQuickSlot StoredQuickSlot;

	UPROPERTY(Replicated)
	FStoredCombatInfo StoredCombatInfo;

	UPROPERTY(Replicated)
	FStoredAttributeDate StoredAttributes;

	UPROPERTY(Replicated)
	bool bShouldRestoreData = false;

	UFUNCTION()
	void CaptureFromComponents(ABasePlayerCharacter* PlayerChar);

	UFUNCTION()
	void RestoreToComponents(ABasePlayerCharacter* PlayerChar);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

};
