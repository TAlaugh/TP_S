// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoWidget.generated.h"

class UItemDataAsset;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TP_S_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UItemDataAsset* ItemData;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RowBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> RowClass;

protected:
	virtual void NativeConstruct() override;
};
