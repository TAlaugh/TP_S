// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Lianhuo_SpawnSkill.generated.h"
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnSkillDelegate,const TArray<AActor*>&, SpawnedSkills);

/**
 * 
 */
UCLASS()
class TP_S_API UAbilityTask_Lianhuo_SpawnSkill : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Task", meta = (DisplayName = "Spawn Skill Task", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_Lianhuo_SpawnSkill* LianhuoSpawnSkill(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AActor> SkillClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius);

	UPROPERTY(BlueprintAssignable)
	FSpawnSkillDelegate OnSpawnFinished;
	
	UPROPERTY(BlueprintAssignable)
	FSpawnSkillDelegate DidNotSpawn;

	//~ Begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~ End UGameplayTask Interface

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AActor> CachedSkillClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;

	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnSkillClassLoaded();
};
