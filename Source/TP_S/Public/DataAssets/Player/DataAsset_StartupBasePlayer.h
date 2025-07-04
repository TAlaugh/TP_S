// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "DataAsset_StartupBasePlayer.generated.h"

struct FPlayerAbilitiySet;

UCLASS()
class TP_S_API UDataAsset_StartupBasePlayer : public UDataAsset_StartupBase
{
	GENERATED_BODY()

protected:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 ApplyLevel = 1) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData", meta = (TitleProperty = "InputTag"))
	TArray<FPlayerAbilitiySet> PlayerStartupAbilitySets;
	
};
