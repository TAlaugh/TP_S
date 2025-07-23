 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyCharacter.h"
#include "Components/UI/EnemyUIcomponent.h"
#include "EnemyCharacter_Lianhuo.generated.h"

struct FInputActionValue;
class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class TP_S_API AEnemyCharacter_Lianhuo : public ABaseEnemyCharacter//,  public IBaseUIInterface
{
	GENERATED_BODY()


public:
	AEnemyCharacter_Lianhuo(const FObjectInitializer& ObjectInitializer);
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
protected:
	
};
