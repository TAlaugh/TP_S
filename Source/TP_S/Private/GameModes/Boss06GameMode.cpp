// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Boss06GameMode.h"

void ABoss06GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ABoss06GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss06GameMode::SetCurrentBoss06GameMode(EBoss06GameModeState InState)
{
	CurrentBoss06GameModeState = InState;

	OnBoss06GameModeStateChanged.Broadcast(CurrentBoss06GameModeState);
	
}
