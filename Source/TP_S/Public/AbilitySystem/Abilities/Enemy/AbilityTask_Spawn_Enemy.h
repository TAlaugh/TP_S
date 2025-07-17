// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Spawn_Enemy.generated.h"


class ABaseEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate,const TArray<ABaseEnemyCharacter*>&,SpawnedEnemies);

/**
 * 
 */
UCLASS()
class TP_S_API UAbilityTask_WaitSpawnEnemy : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EnemyAbilityTask", meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies",HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true", NumToSpawn = "1",RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemy* WaitSpawnEnemies(UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<ABaseEnemyCharacter>SoftEnemyClassToSpawn,
		int32 NumToSpawn,
		const FVector& SpawnOrigin,
		float RandomSpawnRadius
		
		);
	
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;


	//begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//End UGameplayTask Interface
private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<ABaseEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;
	
	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnEnemyClassLoaded();
};
