// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/InventoryMainWidget.h"

#include "Components/Button.h"

void UInventoryMainWidget::Init(UPlayerInventoryComponent* InInventory)
{
	
}

bool UInventoryMainWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (BtnConsumable)
	{
		BtnConsumable->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnConsumableTab);
	}

	if (BtnMelee)
	{
		BtnMelee->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnMeleeTab);
	}

	if (BtnRanged)
	{
		BtnRanged->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnRangedTab);
	}
	
	return true;
}

void UInventoryMainWidget::OnConsumableTab()
{
	
}

void UInventoryMainWidget::OnMeleeTab()
{
}

void UInventoryMainWidget::OnRangedTab()
{
}

void UInventoryMainWidget::Refresh()
{
}
