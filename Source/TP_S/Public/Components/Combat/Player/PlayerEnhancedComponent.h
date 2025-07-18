// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PlayerEnhancedComponent.generated.h"

class ABasePlayerCharacter;

USTRUCT(BlueprintType)
struct FEnhancementOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttributeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};

UCLASS()
class TP_S_API UPlayerEnhancedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerEnhancedComponent();

	UFUNCTION(BlueprintCallable)
	void TriggeredEnhancementSelection(ABasePlayerCharacter* PlayerCharacter);

protected:
	virtual void BeginPlay() override;

	void GetRandomEnhancedOptions(int32 Count, TArray<FEnhancementOption>& OutOptions);
	void ShowEnhancedWidget(const TArray<FEnhancementOption>& Options);
	void ApplyEnhancementToPlayer(const FEnhancementOption& SelectedOption);

protected:
	UPROPERTY(EditAnywhere, Category = "Enhancement")
	TArray<FEnhancementOption> AllEnhancementOptions;

	UPROPERTY(EditAnywhere, Category = "Enhancement")
	TSubclassOf<UUserWidget> EnhancementWidgetClass;
	
private:
	UPROPERTY()
	ABasePlayerCharacter* CurrentPlayer;
	
};
