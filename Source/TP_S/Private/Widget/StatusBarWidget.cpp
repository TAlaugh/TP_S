// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatusBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatusBarWidget::UpdateHp(float Current, float Max)
{
	CurrentHp = Current;
	MaxHp = Max;

	if (HpBar && Max > 0.f)
	{
		HpBar->SetPercent(Current / Max);
	}

	if (HpText)
	{
		FString HpStr = FString::Printf(TEXT("%.0f"), Current);
		HpText->SetText(FText::FromString(HpStr));
	}
}
