// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/BaseGameMode.h"
#include "Boss06GameMode.generated.h"

UENUM(BlueprintType)
enum class EBoss06GameModeState : uint8
{
	WaitSpawnNewWave,
	SpawnNewWave,
	Inprogress,
	WaveComplete,
	AllWavesDone,
	PlayerDied
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnABoss06GameModeStateChangedDelegate, EBoss06GameModeState, CurrentState); 
/**
 * 
 */
UCLASS()
class TP_S_API ABoss06GameMode : public ABaseGameMode
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	void SetCurrentBoss06GameMode(EBoss06GameModeState InState);
	
	UPROPERTY()
	EBoss06GameModeState CurrentBoss06GameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnABoss06GameModeStateChangedDelegate OnBoss06GameModeStateChanged;
};
