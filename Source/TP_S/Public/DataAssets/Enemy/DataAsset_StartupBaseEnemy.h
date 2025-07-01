// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "DataAsset_StartupBaseEnemy.generated.h"

class UEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class TP_S_API UDataAsset_StartupBaseEnemy : public UDataAsset_StartupBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="StartUpData")
	TArray<TSubclassOf<UEnemyGameplayAbility>> EnemyCombatAbilities;
};