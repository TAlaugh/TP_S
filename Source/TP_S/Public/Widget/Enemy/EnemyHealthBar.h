// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/WidgetBase.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class TP_S_API UEnemyHealthBar : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCurrentHPChanged(float NewPercent);

	UFUNCTION()
	void HiddenHealthBar();

protected:
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ProgressBar_HP;

	FTimerHandle HealthTimerHandle;

	bool bIsFirst;
	
};
