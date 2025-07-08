// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"

class UPlayerInventoryComponent;
class UConsumableItemDataAsset;

USTRUCT(BlueprintType)
struct FQuickSlotData
{
	GENERATED_BODY()

	/** 등록된 소비 아이템 **/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UConsumableItemDataAsset> ItemData;

	/** 남은 수량 (인벤토리랑 동기화 용) **/
	UPROPERTY(BlueprintReadOnly)
	int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotChanged, const FQuickSlotData&, NewData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_S_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotChanged OnQuickSlotChanged;

	bool RegisterItem(UConsumableItemDataAsset* Item, UPlayerInventoryComponent* Inventory);

	bool UseSlot(UPlayerInventoryComponent* Inventory);

	const FQuickSlotData& GetData() const { return QuickSlotData; }

private:
	UPROPERTY()
	FQuickSlotData QuickSlotData;
};
