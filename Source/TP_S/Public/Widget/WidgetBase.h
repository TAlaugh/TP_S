// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "WidgetBase.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;

/**
 * 
 */
UCLASS()
class TP_S_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* HeroUIComponent);


	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* EnemyUIComponent);


public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreateWidget(AActor* EnemyActor);
};