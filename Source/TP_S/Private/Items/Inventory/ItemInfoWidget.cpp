// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/ItemInfoWidget.h"

#include "Components/VerticalBox.h"
#include "Items/ItemDataAsset.h"
#include "Items/Inventory/ItemInfoRowWidget.h"

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ItemData || !RowBox || !RowClass) return;

	TArray<FItemInfoRow> Rows;
	ItemData->GatherDisplayRows(Rows);

	for (const FItemInfoRow& R : Rows)
	{
		UItemInfoRowWidget* Row = CreateWidget<UItemInfoRowWidget>(this, RowClass);
		if (Row)
		{
			Row->InitializeRow(R.Label, R.Value);
			RowBox->AddChild(Row);
		}
	}
}
