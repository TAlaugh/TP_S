// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Heavy.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ThrowWeapon.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UPGA_Attack_Melee_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	EquipWeapon();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	/*
	UAbilityTask_PlayMontageAndWait* Task1 = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("Attack"),
			MontageToPlay,
			1.f,
			GetNextSection(),
			false);
	Task1->ReadyForActivation();

	GetWorld()->GetTimerManager().ClearTimer(ReceiveTimer);
	*/
	UBaseFunctionLibrary::AddGameplayTagToActorIfNone(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Player_Status_WeaponThrown);
	ThrowWeapon(FGameplayEventData());
}


void UPGA_Attack_Melee_Heavy::ThrowWeapon(FGameplayEventData Data)
{
	Weapon = CachedPlayerCharacter->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	if (Weapon)
	{
		Task = UAT_Attack_Melee_ThrowWeapon::Init(this, BaseGamePlayTags::Player_Event_Attack_Throw);
		Task->ReadyForActivation();
	
		if (ReceiveMontage && false)
		{
			GetWorld()->GetTimerManager().SetTimer(
				ReceiveTimer,
				FTimerDelegate::CreateLambda([this]()
					{
						CachedPlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ReceiveMontage);
						Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Stop(0.f);
						K2_EndAbility();
					}),
					3.f,
					false);
		}
	}
}

void UPGA_Attack_Melee_Heavy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//UnEquipWeapon(FGameplayEventData());
}

