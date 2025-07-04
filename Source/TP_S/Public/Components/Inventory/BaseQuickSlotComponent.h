// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BaseExtensionComponent.h"
#include "BaseQuickSlotComponent.generated.h"

class UConsumableInventoryComponent;
class UConsumableItem;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotChanged);

UCLASS()
class TP_S_API UBaseQuickSlotComponent : public UBaseExtensionComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 EquippedSlotIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UConsumableInventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotChanged OnQuickSlotChanged;

	UFUNCTION(BlueprintCallable)
	void Initialize(UConsumableInventoryComponent* InConsumableInventoryComponent);

	UFUNCTION(BlueprintCallable)
	void EquipItemByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void UseEquippedItem();
};
