// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "InventoryMainWidget.generated.h"

class USizeBox;
class UItemInfoWidget;
class UItemDataAsset;
class UQuickSlotComponent;
class UInventorySlotWidget;
class UScrollBox;
class UButton;
class UPlayerInventoryComponent;

/**
 * 
 */
UCLASS()
class TP_S_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 플레이어 인벤토리 컴포넌트를 넘겨 초기화 **/
	void Init(UPlayerInventoryComponent* InInventory);

	virtual bool Initialize() override;

	/** 탭 버튼 콜백 **/
	UFUNCTION()
	void OnConsumableTab();

	UFUNCTION()
	void OnMeleeTab();

	UFUNCTION()
	void OnRangedTab();

	/** 인벤토리 변경 시 호출, 델리게이트 연결 **/
	UFUNCTION()
	void Refresh();

	/** BindWidget **/
	UPROPERTY(meta = (BindWidget))
	UButton* BtnConsumable;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnMelee;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnRanged;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollItems;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> SlotClass;

	UPROPERTY()
	UInventorySlotWidget* SelectedSlot;

	UFUNCTION()
	void HandleSlotClicked(UInventorySlotWidget* Clicked);
	
	void SetSelectedSlot(UInventorySlotWidget* NewSlot);

	UFUNCTION()
	void HandleQuickSlotChanged(const FQuickSlotData& Data);

	/** 아이템 정보 표시 **/
	UPROPERTY(meta = (BindWidget))
	USizeBox* InfoWidgetHolder;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Class")
	TSubclassOf<UItemInfoWidget> InfoWidgetClass;

	UPROPERTY()
	UItemInfoWidget* CurrentInfoWidget;

	UFUNCTION()
	void ShowItemInfo(UItemDataAsset* ItemData);
	
private:
	UPROPERTY()
	UPlayerInventoryComponent* Inventory;

	UPROPERTY()
	UQuickSlotComponent* QuickSlotComponent;
	
	EInventoryCategory CurrentTab = EInventoryCategory::Consumable;
};
