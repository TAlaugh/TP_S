// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/PlayerGameplayAbility_Attack.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerGameplayAbility_Attack::UPlayerGameplayAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPlayerGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerCharacterFromActorInfo() && GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->CurrentEquippedWeaponTag.IsValid())
	{
		//Debug::Print(GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->CurrentEquippedWeaponTag.ToString());		
	}
}

void UPlayerGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UPlayerGameplayAbility_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UPlayerGameplayAbility_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPlayerGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_Attack::EquipWeapon(FName SocketName)
{
}

void UPlayerGameplayAbility_Attack::UnEquipWeapon(FGameplayEventData TargetData = FGameplayEventData())
{
}

void UPlayerGameplayAbility_Attack::EquipWeaponLeftSocket(FGameplayEventData Data)
{
}

void UPlayerGameplayAbility_Attack::EquipWeaponRightSocket(FGameplayEventData Data)
{
}

void UPlayerGameplayAbility_Attack::OnCompleteCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UPlayerGameplayAbility_Attack::GetNextSection()
{
	return *FString::Printf(TEXT("Default"));
}


