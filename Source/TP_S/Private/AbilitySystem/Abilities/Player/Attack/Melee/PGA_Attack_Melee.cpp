// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effects/GE/GE_DealDamage.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

void UPGA_Attack_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	EquipWeapon();
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("Attack"),
			MontageToPlay,
			1.f,
			GetNextSection(),
			false);
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* DamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Shared_Event_Hit);
	DamageTask->EventReceived.AddDynamic(this, &ThisClass::HandleApplyDamage);
	DamageTask->ReadyForActivation();
}

void UPGA_Attack_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGA_Attack_Melee::HandleApplyDamage(FGameplayEventData Data)
{
	AActor* TargetActor = const_cast<AActor*>(Data.Target.Get());
	if (IsValid(TargetActor))
	{
		
		TSubclassOf<UGameplayEffect> Effect = UGE_DealDamage::StaticClass();
		float BaseDamage = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
		int ComboCount = 0;
		FGameplayEffectSpecHandle SpecHandle = MakePlayerDamageGameplayEffectHandle(
			Effect,
			BaseDamage,
			BaseGamePlayTags::Player_Ability_Attack_Melee_Light,
			ComboCount);
		NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BaseGamePlayTags::Shared_Event_HitReact, Data);
	}
}

void UPGA_Attack_Melee::EquipWeapon(FName SocketName)
{
	GetPlayerCombatComponentFromActorInfo()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee, SocketName);
}

void UPGA_Attack_Melee::UnEquipWeapon(FGameplayEventData TargetData)
{
	GetPlayerCombatComponentFromActorInfo()->UnEquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
}

void UPGA_Attack_Melee::EquipWeaponLeftSocket(FGameplayEventData Data)
{
	EquipWeapon(FName("hand_lSocket"));
}

void UPGA_Attack_Melee::EquipWeaponRightSocket(FGameplayEventData Data)
{
	EquipWeapon(FName("hand_rSocket"));
}
