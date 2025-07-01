// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_S/Public/Character/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/UI/BaseUIComponent.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "Interfaces/BaseCombatInterface.h"
#include "Interfaces/BaseUIInterface.h"
#include "BaseEnemyCharacter.generated.h"



class UEnemyCombatComponent;
class UBaseCombatComponent;
class UMotionWarpingComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class TP_S_API ABaseEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	ABaseEnemyCharacter();

	public:

	protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StartupData")
	TSoftObjectPtr<UDataAsset_StartupBase> StartupData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEnemyCombatComponent* BaseEnemyCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UBaseUIComponent* EnemyUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* WidgetComponent;


	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual UBaseCombatComponent* GetBaseCombatComponent() const;// override;
	virtual UBaseUIComponent* GetBaseUIComponent() const; //override;
	virtual UBaseUIComponent* GetEnemyUIComponent() const;// override;
	

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetBaseEnemyCombatComponent() const {return BaseEnemyCombatComponent;}
};
