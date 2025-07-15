// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/BaseGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UBaseGameInstance::AdvanceStage()
{
	CurrentStageCount++;
	bBossStage = (CurrentStageCount >= 3);
}

void UBaseGameInstance::AdvancedFloor()
{
	CurrentFloorCount++;
	CurrentStageCount = 0;
	bBossStage = false;
}