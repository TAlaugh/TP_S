// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BasePlayerController.h"

ABasePlayerController::ABasePlayerController()
{
	TeamId = FGenericTeamId(0);
}

FGenericTeamId ABasePlayerController::GetGenericTeamId() const
{
	return TeamId;
}
