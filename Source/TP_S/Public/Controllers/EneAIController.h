// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Patrol      UMETA(DisplayName = "Patrol"),
	Investigate UMETA(DisplayName = "Investigate"),
	Alert       UMETA(DisplayName = "Alert"),
	Combat      UMETA(DisplayName = "Combat"),
	Flee        UMETA(DisplayName = "Flee")
};

#include "CoreMinimal.h"
#include "AIController.h"
#include "EneAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

/**
 * 
 */

UCLASS()
class TP_S_API AEneAIController : public AAIController
{
	GENERATED_BODY()

	AEneAIController(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Damage* AISenseConfig_Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	
	UFUNCTION(BlueprintCallable, Category="AI|Perception")
	void HandleDamageStimulus(AActor* Actor, FAIStimulus Stimulus);
   
	void AdjustPerceptionForState(EAIState State);
	
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	bool bDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition= "bDetourCrowdAvoidance", UIMin="1", UIMax="4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition="bDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;

	
};

