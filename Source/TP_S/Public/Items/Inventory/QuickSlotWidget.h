// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "QuickSlotWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TP_S_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Update(const FQuickSlotData& Data);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ImageIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtCount;

	virtual void NativeConstruct() override;
};
