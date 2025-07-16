// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/BaseUIComponent.h"

#include "Widget/WidgetBase.h"

void UBaseUIComponent::RegisterEnemyDrawnWidget(UWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UBaseUIComponent::RemoveEnemyDrawnWidgetIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}
	for (UWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
