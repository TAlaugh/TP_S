// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemAcquiredWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class TP_S_API UItemAcquiredWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitItemWidget(FText ItemName, UMaterialInterface* ItemIcon);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;
};
