// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/ItemInfoRowWidget.h"

#include "Components/TextBlock.h"

void UItemInfoRowWidget::InitializeRow(const FText& InLabel, const FText& InValue)
{
	if (LabelText)
	{
		LabelText->SetText(InLabel);
	}
	if (ValueText)
	{
		ValueText->SetText(InValue);
	}
}
