// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FrontendController.h"

#include "Camera/CameraActor.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void AFrontendController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTargetWithBlend(FoundCameras[0]);
	}

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();

	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
