// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/AbilityTask_Spawn_Enemy.h"
#include "AbilitySystemComponent.h"
#include "DebugHelper.h"
#include "NavigationSystem.h"
#include "Character/Enemy/BaseEnemyCharacter.h"
#include "Engine/AssetManager.h"
UAbilityTask_WaitSpawnEnemy* UAbilityTask_WaitSpawnEnemy::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
                                                                               FGameplayTag EventTag, TSoftClassPtr<ABaseEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn,
                                                                               const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemy* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemy>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	

	return Node;
}

void UAbilityTask_WaitSpawnEnemy::Activate()
{
	Super::Activate();
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle = Delegate.AddUObject(this,&ThisClass::OnGameplayEventReceived); 
}

void UAbilityTask_WaitSpawnEnemy::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(DelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemy::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        		CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
        		FStreamableDelegate::CreateUObject(this,&ThisClass::OnEnemyClassLoaded)
        		);
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<ABaseEnemyCharacter*>());
			
		}
		EndTask();
	}
	
}

void UAbilityTask_WaitSpawnEnemy::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<ABaseEnemyCharacter*>());
		}
		EndTask();
		
		return;
	}
	TArray<ABaseEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (int32 i = 0; i< CachedNumToSpawn; i++)
	{
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this,CachedSpawnOrigin,RandomLocation,CachedRandomSpawnRadius);

		RandomLocation +=FVector(0.f,0.f,150.f);
		
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		ABaseEnemyCharacter* SpawnedEnemy = World->SpawnActor<ABaseEnemyCharacter>(LoadedClass,RandomLocation,SpawnFacingRotation,SpawnParams);

		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}

	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<ABaseEnemyCharacter*>());
		}
	}
	EndTask();
}
