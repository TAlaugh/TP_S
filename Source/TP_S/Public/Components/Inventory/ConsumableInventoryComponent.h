// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/BaseInventoryComponent.h"
#include "ConsumableInventoryComponent.generated.h"

class UConsumableItem;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FConsumableInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UConsumableItem> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownEndTime = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS()
class TP_S_API UConsumableInventoryComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FConsumableInventorySlot> InventorySlots;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable)
	void AddItem(UConsumableItem* Item, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	void ConsumeItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool IsOnCooldown(const FConsumableInventorySlot& Slot) const;

	UFUNCTION(BlueprintCallable)
	const FConsumableInventorySlot& GetSlot(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool IsValidIndex(int32 Index) const;
};
