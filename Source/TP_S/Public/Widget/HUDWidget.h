// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UAimWidget;
class UItemDataAsset;
class UItemAcquiredContainerWidget;
class UWeaponHUDWidget;
class UWeaponItemDataAsset;
class UQuickSlotComponent;
class UQuickSlotWidget;
class UStatusBarWidget;
class UBaseAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TP_S_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAttribute(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet);
	
	void BindToQuickSlot(UQuickSlotComponent* QSC);
	
	void OnCurrentHpChanged(const FOnAttributeChangeData& Data);
	void OnMaxHpChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable)
	void NotifyItemAcquired(UItemDataAsset* ItemAsset, int32 Amount);

	UWeaponHUDWidget* GetWeaponHUDWidget() const { return WeaponHUDWidget; }
	UQuickSlotWidget* GetQuickSlotWidget() const { return QuickSlotWidget; }
	UAimWidget* GetAimWidget() const { return AimWidget; }
	
protected:
	UPROPERTY(meta = (BindWidget))
	UStatusBarWidget* StatusBarWidget;

	UPROPERTY(meta = (BindWidget))
	UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(meta = (BindWidget))
	UWeaponHUDWidget* WeaponHUDWidget;

	UPROPERTY(meta = (BindWidget))
	UAimWidget* AimWidget;

	UPROPERTY(meta = (BindWidget))
	UItemAcquiredContainerWidget* ItemNotifyContainer;

	UPROPERTY()
	UBasePlayerCombatComponent* BoundCombatComponent;

	float CachedCurrentHp = 0.f;
	float CachedMaxHp = 0.f;
	
};
