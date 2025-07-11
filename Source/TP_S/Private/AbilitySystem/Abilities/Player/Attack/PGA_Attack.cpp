// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/PGA_Attack.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPGA_Attack::UPGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerCharacterFromActorInfo() && GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->CurrentEquippedWeaponTag.IsValid())
	{
		//Debug::Print(GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->CurrentEquippedWeaponTag.ToString());		
	}
}

void UPGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UPGA_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UPGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGA_Attack::EquipWeapon(FName SocketName)
{
}

void UPGA_Attack::UnEquipWeapon(FGameplayEventData TargetData = FGameplayEventData())
{
}

void UPGA_Attack::EquipWeaponLeftSocket(FGameplayEventData Data)
{
}

void UPGA_Attack::EquipWeaponRightSocket(FGameplayEventData Data)
{
}

FName UPGA_Attack::GetNextSection()
{
	return *FString::Printf(TEXT("Default"));
}


