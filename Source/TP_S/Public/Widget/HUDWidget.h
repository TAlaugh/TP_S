// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

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

	void OnCurrentHpChanged(const FOnAttributeChangeData& Data);
	void OnMaxHpChanged(const FOnAttributeChangeData& Data);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UStatusBarWidget* StatusBarWidget;

	float CachedCurrentHp = 0.f;
	float CachedMaxHp = 0.f;
};
