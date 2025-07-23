// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemAcquiredContainerWidget.generated.h"

class UItemAcquiredWidget;
class UVerticalBox;
class UItemDataAsset;
/**
 * 
 */
UCLASS()
class TP_S_API UItemAcquiredContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddAcquiredItem(UItemDataAsset* ItemDataAsset, int32 Amount);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* BoxItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAcquired")
	TSubclassOf<UItemAcquiredWidget> ItemWidgetClass;
};
