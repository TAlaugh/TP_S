// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UFrontendGameUserSettings* Get();

	// Gameplay Collection Tab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
