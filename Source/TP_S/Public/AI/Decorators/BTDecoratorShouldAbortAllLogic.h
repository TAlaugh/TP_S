// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecoratorShouldAbortAllLogic.generated.h"

/**
 * 
 */
UCLASS()
 class TP_S_API UBTDecoratorShouldAbortAllLogic : public UBTDecorator
 {
 	GENERATED_BODY()
public:
	UBTDecoratorShouldAbortAllLogic();

protected:
 	UFUNCTION(BlueprintCallable, Category="AI")
 	virtual bool PerformConditionCheckAI(UBehaviorTreeComponent* OwnerComp, AAIController* OwnerController, APawn* ControlledPawn);

public:
 	UPROPERTY(VisibleAnywhere, Category="AI")
 	FBlackboardKeySelector InTargetActorKey;

 	UPROPERTY(VisibleAnywhere, Category="AI")
 	FBlackboardKeySelector InDistToTargetKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName DeadTag = TEXT("Shared.Status.Dead");
 };
