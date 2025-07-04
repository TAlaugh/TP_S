// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

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

protected:
	virtual bool Initialize() override;

	/** 탭 버튼 콜백 **/
	UFUNCTION()
	void OnConsumableTab();

	UFUNCTION()
	void OnMeleeTab();

	UFUNCTION()
	void OnRangedTab();

	/** 인벤토리 변경 시 호출 **/
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

private:
	UPROPERTY()
	UPlayerInventoryComponent* Inventory;
	EInventoryCategory CurrentTab = EInventoryCategory::Consumable;
};
