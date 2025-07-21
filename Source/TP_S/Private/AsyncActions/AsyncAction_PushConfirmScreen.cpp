// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/AsyncAction_PushConfirmScreen.h"

#include "GameInstance/Subsystems/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                                  EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMsg)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushConfirmScreen* Node = NewObject<UAsyncAction_PushConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMsg = InScreenMsg;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	UFrontendUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMsg,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		}
	);
}
