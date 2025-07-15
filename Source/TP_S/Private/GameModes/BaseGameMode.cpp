// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BaseGameMode.h"

#include "GameInstance/BaseGameInstance.h"

ABaseGameMode::ABaseGameMode()
{
	bUseSeamlessTravel = true;
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());

	if (GI && GI->bBossStage)
	{
		SpawnBoss();
	}
	else
	{
		SpawnMonster();
	}
}

void ABaseGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	// UE_LOG(LogTemp, Warning, TEXT("[GameMode] PostSeamlessTravel called"));
}

void ABaseGameMode::SpawnMonster()
{
	
}

void ABaseGameMode::SpawnBoss()
{
	
}
