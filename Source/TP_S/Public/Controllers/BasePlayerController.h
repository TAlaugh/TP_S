// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UHUDWidget;
class UQuickSlotComponent;
class UQuickSlotWidget;
class UInputAction;
class UInputMappingContext;
class UPlayerInventoryComponent;
class UInventoryMainWidget;
/**
 * 
 */
UCLASS()
class TP_S_API ABasePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABasePlayerController();

	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId TeamId;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// 인벤토리 열기
	void OnToggleInventory(const FInputActionValue& Value);

	// 내부 유틸
	void ShowInventory();
	void HideInventory();

	// 퀵슬롯 사용
	void OnUseQuickSlot(const FInputActionValue& Value);

	// Debug
	UFUNCTION(BlueprintCallable)
	void GiveItems();

	// 키 바인딩
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> IMC_Default;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_ToggleInventory;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_UseQuickSlot;
	
	// 위젯 클래스 BP지정 
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> QuickSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;
	
private:
	UPROPERTY()
	UInventoryMainWidget* InventoryWidget;

	UPROPERTY()
	UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY()
	UHUDWidget* PlayerHUDWidget;
	
	UPROPERTY()
	UPlayerInventoryComponent* InventoryComponent;

	UPROPERTY()
	UQuickSlotComponent* QuickSlotComponent;

	bool bInventoryOpen = false;
};
