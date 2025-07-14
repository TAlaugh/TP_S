// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/BaseGameInstance.h"

void UBaseGameInstance::AdvanceStage()
{
	CurrentStageCount++;
	bBossStage = (CurrentStageCount >= 3);
}
