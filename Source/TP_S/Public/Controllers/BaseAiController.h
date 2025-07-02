// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAiController.generated.h"


struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
class UAISenseConfig_Team;
/**
 * 
 */
UCLASS()
class TP_S_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

	ABaseAIController(const FObjectInitializer& ObjectInitializer) ;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Hearing* AISenseConfig_Hearing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Damage* AISenseConfig_Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Team* AISenseConfig_Team;
	
	UFUNCTION()
	
	virtual void BeginPlay() override;
	
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	
private:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	bool bDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition="bDetourCrowdAvoidance", UIMin="1",UIMax="4"))
	int32 DetourCrowdAvoidanceQuality = 4;


	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition="bDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;
};
