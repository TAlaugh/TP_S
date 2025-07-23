// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "GameInstance/Subsystems/FrontendLoadingScreenSubsystem.h"
// #include "Tickable.h"
// #include "DebugHelper.h"
// #include "PreLoadScreenManager.h"
// #include "Blueprint/UserWidget.h"
// #include "FrontendSettings/Interfaces/FrontendLoadingScreenInterface.h"
// #include "FrontendSettings/FrontendLoadingScreenSettings.h"
//
// bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
// {
// 	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
// 	{
// 		TArray<UClass*> FoundClasses;
// 		GetDerivedClasses(GetClass(), FoundClasses);
//
// 		return FoundClasses.IsEmpty();
// 	}
// 	
// 	return false;
// }
//
// void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
// {
// 	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
// 	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
// }
//
// void UFrontendLoadingScreenSubsystem::Deinitialize()
// {
// 	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
// 	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
// }
//
// UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
// {
// 	if (UGameInstance* OwningGameInstance = GetGameInstance())
// 	{
// 		return OwningGameInstance->GetWorld();
// 	}
// 	
// 	return nullptr;
// }
//
// void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
// {
// 	TryUpdateLoadingScreen();
// }
//
// ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
// {
// 	if (IsTemplate())
// 	{
// 		return ETickableTickType::Never;
// 	}
// 	
// 	return CurrentTickType;
// }
//
// bool UFrontendLoadingScreenSubsystem::IsTickable() const
// {
// 	return GetGameInstance() && GetGameInstance()->GetGameViewportClient() && CurrentTickType != ETickableTickType::Never;
// }
//
// TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
// {
// 	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
// }
//
// void UFrontendLoadingScreenSubsystem::SetTickableTickType(ETickableTickType NewTickType)
// {
// 	CurrentTickType = NewTickType;
// }
//
// void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
// {
// 	if (WorldContext.OwningGameInstance != GetGameInstance())
// 	{
// 		return;
// 	}
//
// 	SetTickableTickType(ETickableTickType::Conditional);
//
// 	bIsCurrentlyLoadingMap = true;
//
// 	TryUpdateLoadingScreen();
// }
//
// void UFrontendLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
// {
// 	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
// 	{
// 		bIsCurrentlyLoadingMap = false;
// 	}
// }
//
// void UFrontendLoadingScreenSubsystem::TryUpdateLoadingScreen()
// {
// 	// check if there's any start up loading screen that's currently active
// 	if (IsPreLoadScreenActive())
// 	{
// 		return;
// 	}
// 	
// 	// check if we should show the loading screen
// 	if (ShouldShowLoadingScreen())
// 	{
// 		// try display the loading screen here
// 		TryDisplayLoadingScreenIfNone();
// 		
// 		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
// 	}
// 	else
// 	{
// 		// Try remove the current active loading screen
// 		TryRemoveLoadingScreen();
//
// 		HoldLoadingScreenStartUpTime = -1.f;
// 		
// 		// Notify the loading is complete
//
// 		// Disable the ticking
// 		// SetTickableTickType(ETickableTickType::Never);
// 		CurrentTickType = ETickableTickType::Never;
// 	}
// }
//
// bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive() const
// {
// 	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
// 	{
// 		return PreLoadScreenManager->HasValidActivePreLoadScreen();
// 	}
// 	
// 	return false;
// }
//
// bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
// {
// 	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
//
// 	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
// 	{
// 		return false;
// 	}
//
// 	// Check if the objects in the world need a loading screen
//
// 	if (CheckTheNeedToShowLoadingScreen())
// 	{
// 		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
//
// 		return true;
// 	}
//
// 	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");
//
// 	// there's no need to show the loading screen. allow the world to be rendered to our vieport here
// 	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
//
// 	const float CurrentTime = FPlatformTime::Seconds();
//
// 	if (HoldLoadingScreenStartUpTime < 0.f)
// 	{
// 		HoldLoadingScreenStartUpTime = CurrentTime;
// 	}
//
// 	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;
//
// 	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
// 	{
// 		return true;	
// 	}
// 	
// 	return false;
// }
//
// bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
// {
// 	if (bIsCurrentlyLoadingMap)
// 	{
// 		CurrentLoadingReason = TEXT("Loading Level");
// 		
// 		return true;
// 	}
//
// 	UWorld* OwningWorld = GetGameInstance()->GetWorld();
//
// 	if (!OwningWorld)
// 	{
// 		CurrentLoadingReason = TEXT("Loading World");
//
// 		return true;
// 	}
//
// 	if (!OwningWorld->HasBegunPlay())
// 	{
// 		CurrentLoadingReason = TEXT("World hasn't begun play yet");
//
// 		return true;
// 	}
//
// 	if (!OwningWorld->GetFirstPlayerController())
// 	{
// 		CurrentLoadingReason = TEXT("Player Controller is not valid yet");
//
// 		return true;
// 	}
//
// 	// check if the game states, player states, or player character, actor component are ready
// 	
// 	return false;
// }
//
// void UFrontendLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
// {
// 	// if there's already active loading screen, return if yes
//
// 	if (CachedCreatedLoadingScreenWidget)
// 	{
// 		return;
// 	}
// 	
// 	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
//
// 	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();
//
// 	UUserWidget* CreateWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);
//
// 	check(CreateWidget);
//
// 	CachedCreatedLoadingScreenWidget = CreateWidget->TakeWidget();
// 	
// 	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
// 		CachedCreatedLoadingScreenWidget.ToSharedRef(),
// 		1000
// 	);
//
// 	NotifyLoadingScreenVisibilityChanged(true);
// }
//
// void UFrontendLoadingScreenSubsystem::TryRemoveLoadingScreen()
// {
// 	if (!CachedCreatedLoadingScreenWidget)
// 	{
// 		return;
// 	}
//
// 	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());
//
// 	CachedCreatedLoadingScreenWidget.Reset();
//
// 	NotifyLoadingScreenVisibilityChanged(false);
// }
//
// void UFrontendLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bVisible)
// {
// 	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
// 	{
// 		if (!ExistingLocalPlayer)
// 		{
// 			continue;
// 		}
//
// 		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
// 		{
// 			if (PC->Implements<UFrontendLoadingScreenInterface>())
// 			{
// 				if (bVisible)
// 				{
// 					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
// 				}
// 				else
// 				{
// 					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
// 				}
// 			}
//
// 			if (APawn* OwningPawn = PC->GetPawn())
// 			{
// 				if (OwningPawn->Implements<UFrontendLoadingScreenInterface>())
// 				{
// 					if (bVisible)
// 					{
// 						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
// 					}
// 					else
// 					{
// 						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
// 					}
// 				}
// 			}
// 		}
// 	}
// }
