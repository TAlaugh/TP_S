// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TP_S_API UStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealth(float Current, float Max);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HpText;

	float CurrentHp = 0.f;
	float MaxHp = 0.f;
};
