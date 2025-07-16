// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/AbilityTask_Lianhuo_SpawnSkill.h"

UAbilityTask_Lianhuo_SpawnSkill* UAbilityTask_Lianhuo_SpawnSkill::LianhuoSpawnSkill(UGameplayAbility* OwningAbility,
	FGameplayTag EventTag, TSoftClassPtr<AEnemyCharacter_Lianhuo> SkillClassToSpawn, int32 NumToSpawn,
	const FVector& SpawnOrigin, float RandomSpawnRadius, const FRotator& SpawnRotation)
{
	UAbilityTask_Lianhuo_SpawnSkill* Node = NewAbilityTask<UAbilityTask_Lianhuo_SpawnSkill>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSkillClassToSpawn = SkillClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	Node->CachedSpawnRotation = SpawnRotation;
	
	return Node;
}
