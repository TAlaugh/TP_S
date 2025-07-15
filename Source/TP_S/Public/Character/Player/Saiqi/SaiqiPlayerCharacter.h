// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/BaseUIInterface.h"
#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"
#include "SaiqiPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API ASaiqiPlayerCharacter : public ABasePlayerCharacter, public IBaseUIInterface
{
	GENERATED_BODY()

public:
	ASaiqiPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual UBaseUIComponent* GetBaseUIComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UBaseUIComponent* BaseUIComponent;
};
