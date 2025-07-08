// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/BaseInventoryComponent.h"
#include "Items/ItemDataAsset.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

	/** 어떤 아이템 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> ItemData;

	/** 개수 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class TP_S_API UPlayerInventoryComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()

public:
	/** 델리게이트 **/
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	/** 슬롯 배열 접근 **/
	const TArray<FItemStack>& GetStacks() const { return Items; }

	/** 카테고리 필터 **/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetItemsByCategory(EInventoryCategory Category, TArray<FItemStack>& OutStacks) const;

	/** 아이템 추가 | 삭제 **/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemDataAsset* Item, int32 Qty = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveAt(int32 Index, int32 Qty = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveOne(UItemDataAsset* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 CountItem(UItemDataAsset* Item) const;
	
private:
	/** 실제 저장소 **/
	UPROPERTY()
	TArray<FItemStack> Items;

	/** 내부 유틸 **/
	int32 FindStackIndex(UItemDataAsset* Item) const;
	
};
