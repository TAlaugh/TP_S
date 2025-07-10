// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Light.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

void UPGA_Attack_Melee_Light::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	//EquipWeapon();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bIsAttacking = true;
	UAbilityTask_WaitGameplayEvent* TaskToNext = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Next
		);
	TaskToNext->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::NextCombo);
	TaskToNext->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* TaskToEquip_Left = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Equip_Left);
	TaskToEquip_Left->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::EquipWeaponLeftSocket);
	TaskToEquip_Left->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* TaskToEquip_Right = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Equip_Right);
	TaskToEquip_Right->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::EquipWeaponRightSocket);
	TaskToEquip_Right->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* TaskToFinish = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Finish
		);
	TaskToFinish->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::StopAttack);
	TaskToFinish->ReadyForActivation();
	
}

void UPGA_Attack_Melee_Light::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	HasNextComboInput = true;
	bIsAttacking = true;
}

void UPGA_Attack_Melee_Light::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([this]()
			{
				UnEquipWeapon(FGameplayEventData());
			}),
			1.f,
			false);
	}
	else
	{
		TimerHandle.Invalidate();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentCombo = 0;
	HasNextComboInput = false;
}

FName UPGA_Attack_Melee_Light::GetNextSection()
{
	CurrentCombo++;
	if (CurrentCombo > 5)
	{
		CurrentCombo = 1;
	}
	return *FString::Printf(TEXT("%d"), CurrentCombo);
}


void UPGA_Attack_Melee_Light::NextCombo(FGameplayEventData TargetData)
{
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		HasNextComboInput = false;
	}
}

void UPGA_Attack_Melee_Light::StopAttack(FGameplayEventData Data)
{
	if (!bIsAttacking)
	{
		K2_CancelAbility();
	}
}
