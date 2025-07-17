// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/AbilityTask_Lianhuo_SpawnSkill.h"
#include "AbilitySystemComponent.h"
#include "DebugHelper.h"
#include "NavigationSystem.h"
#include "Character/Enemy/EnemyCharacter_Lianhuo.h"
#include "Engine/AssetManager.h"

UAbilityTask_Lianhuo_SpawnSkill* UAbilityTask_Lianhuo_SpawnSkill::LianhuoSpawnSkill(UGameplayAbility* OwningAbility,
                                                                                    FGameplayTag EventTag, TSoftClassPtr<AActor> SkillClassToSpawn, int32 NumToSpawn,
                                                                                    const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_Lianhuo_SpawnSkill* Node = NewAbilityTask<UAbilityTask_Lianhuo_SpawnSkill>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSkillClassToSpawn = SkillClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	
	return Node;
}

void UAbilityTask_Lianhuo_SpawnSkill::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_Lianhuo_SpawnSkill::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	
	Delegate.Remove(DelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_Lianhuo_SpawnSkill::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSkillClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSkillClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnSkillClassLoaded)
			);
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AActor*>());
		}

		EndTask();
	}
}

void UAbilityTask_Lianhuo_SpawnSkill::OnSkillClassLoaded()
{
	UClass* LoadedClass = CachedSkillClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AActor*>());
		}

		EndTask();

		return;
	}

	TArray<AActor*> SpawnedSkills;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < CachedNumToSpawn; ++i)
	{
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(
			this,
			CachedSpawnOrigin,
			RandomLocation,
			CachedRandomSpawnRadius
		);

		RandomLocation += FVector(0.f, 0.f, 0.f);

		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		AActor* SpawnedSkill = World->SpawnActor<AActor>(
			LoadedClass,
			RandomLocation,
			SpawnFacingRotation,
			SpawnParam
		);

		if (SpawnedSkill)
		{
			SpawnedSkills.Add(SpawnedSkill);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedSkills.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedSkills);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AActor*>());
		}
	}
	
	EndTask();
}
