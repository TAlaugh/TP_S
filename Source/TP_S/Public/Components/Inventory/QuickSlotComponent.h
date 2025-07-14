// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"

class UPlayerInventoryComponent;
class UConsumableItemDataAsset;

USTRUCT(BlueprintType)
struct FQuickSlotData
{
	GENERATED_BODY()

	// 등록된 소비 아이템 
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UConsumableItemDataAsset> ItemData;

	// 남은 수량 (인벤토리랑 동기화 용)
	UPROPERTY(BlueprintReadOnly)
	int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotChanged, const FQuickSlotData&, NewData);

UCLASS()
class TP_S_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleInventoryChanged();
	
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotChanged OnQuickSlotChanged;

	bool RegisterItem(UConsumableItemDataAsset* Item, UPlayerInventoryComponent* Inventory);

	UFUNCTION(BlueprintCallable)
	bool UseSlot(UPlayerInventoryComponent* Inventory);

	const FQuickSlotData& GetData() const { return QuickSlotData; }

	// 저장하기 위한 함수
	const FQuickSlotData& GetQuickSlotData() const;
private:
	UPROPERTY()
	FQuickSlotData QuickSlotData;
};
