// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BaseExtensionComponent.h"
#include "BaseUIComponent.generated.h"

class UWidgetBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate, float, NewPercent);
/**
 * 
 */
UCLASS()
class TP_S_API UBaseUIComponent : public UBaseExtensionComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentHpChanged;
	
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UWidgetBase* InWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetIfAny();
private:
	TArray<UWidgetBase*> EnemyDrawnWidgets;
};
