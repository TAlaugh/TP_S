// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoRowWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TP_S_API UItemInfoRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ValueText;

	void InitializeRow(const FText& InLabel, const FText& InValue);
};
