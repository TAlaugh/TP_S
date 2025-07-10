// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDWidget.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Widget/StatusBarWidget.h"

void UHUDWidget::BindToAttribute(UAbilitySystemComponent* ASC, UBaseAttributeSet* AttributeSet)
{
	if (!ASC || !AttributeSet || !StatusBarWidget) return;

	CachedCurrentHp = AttributeSet->GetCurrentHp();
	CachedMaxHp = AttributeSet->GetMaxHp();
	StatusBarWidget->UpdateHp(CachedCurrentHp, CachedMaxHp);

	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHpAttribute()).AddUObject(this, &UHUDWidget::OnCurrentHpChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHpAttribute()).AddUObject(this, &UHUDWidget::OnMaxHpChanged);
}

void UHUDWidget::OnCurrentHpChanged(const FOnAttributeChangeData& Data)
{
	CachedCurrentHp = Data.NewValue;
	StatusBarWidget->UpdateHp(CachedCurrentHp, CachedMaxHp);
}

void UHUDWidget::OnMaxHpChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxHp = Data.NewValue;
	StatusBarWidget->UpdateHp(CachedCurrentHp, CachedMaxHp);
}
