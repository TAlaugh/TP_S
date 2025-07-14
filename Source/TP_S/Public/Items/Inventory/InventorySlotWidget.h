// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;
class UButton;
class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, class UInventorySlotWidget*, ClickedSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClicked, class UItemDataAsset*, ClickedItem);

UCLASS()
class TP_S_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 슬롯에 아이템과 수량을 세팅
	UFUNCTION(BlueprintCallable)
	void SetupSlot(UItemDataAsset* InItemData, int32 InCount);

	// 클릭 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;
	
	virtual bool Initialize() override;

	// BindWiget
	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
	UImage* IConImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	UImage* Selected_BG;

	UPROPERTY(meta = (BindWidget))
	UImage* Circle_Selected_BG;

	// 슬롯 선택 시 강조 부분
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bNewSelected);

	bool IsSelected() const { return bIsSelected; }

	bool bIsSelected = false;

	// 퀵 슬롯 등록 시 강조
	UPROPERTY(meta = (BindWidget))
	UImage* QuickSlot_BG;

	void SetQuickSlotBG(bool bOn);

	// 클릭 시 호출
	UFUNCTION()
	void HandleOnClicked();

	UPROPERTY()
	UItemDataAsset* ItemData;

	UPROPERTY()
	int32 Count;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItemDataAsset* GetItem() const { return ItemData; }

	// 아이템 정보 위젯
	UPROPERTY(BlueprintAssignable) FOnItemClicked OnItemClicked;
	
};
