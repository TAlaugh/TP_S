// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/PlayerEnhancedComponent.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Widget/EnhancementChoiceWidget.h"

UPlayerEnhancedComponent::UPlayerEnhancedComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerEnhancedComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerEnhancedComponent::TriggeredEnhancementSelection(ABasePlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	CurrentPlayer = PlayerCharacter;

	TArray<FEnhancementOption> Options;
	GetRandomEnhancedOptions(3, Options);
	ShowEnhancedWidget(Options);
}

void UPlayerEnhancedComponent::GetRandomEnhancedOptions(int32 Count, TArray<FEnhancementOption>& OutOptions)
{
	OutOptions.Empty();

	TArray<FEnhancementOption> Pool = AllEnhancementOptions;

	for (int32 i = 0; i < Count && Pool.Num() > 0; ++i)
	{
		int32 Index = FMath::RandRange(0,Pool.Num() - 1);
		OutOptions.Add(Pool[Index]);
		Pool.RemoveAt(Index);
	}
}

void UPlayerEnhancedComponent::ShowEnhancedWidget(const TArray<FEnhancementOption>& Options)
{
	if (!EnhancementWidgetClass || !CurrentPlayer) return;

	UEnhancementChoiceWidget* Widget = CreateWidget<UEnhancementChoiceWidget>(GetWorld(), EnhancementWidgetClass);
	if (!Widget) return;

	// TODO
	Widget->AddToViewport();
}

void UPlayerEnhancedComponent::ApplyEnhancementToPlayer(const FEnhancementOption& SelectedOption)
{
	
}

