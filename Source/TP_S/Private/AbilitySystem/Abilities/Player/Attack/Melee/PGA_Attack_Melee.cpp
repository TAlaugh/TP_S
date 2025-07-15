// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effects/GE/GE_DealDamage.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

UPGA_Attack_Melee::UPGA_Attack_Melee()
{
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Range);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Shared_Status_Crouch);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement);

	WeaponType = BaseGamePlayTags::Item_Equipable_Weapon_Melee;
	WeaponSocketName = FName("hand_rSocket");
}

void UPGA_Attack_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("Attack"),
			MontageToPlay,
			1.f,
			GetNextSection(),
			true);
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
		int ComboCount = CachedCurrentSection;
		FGameplayEffectSpecHandle SpecHandle = MakePlayerDamageGameplayEffectHandle(
			Effect,
			BaseDamage,
			BaseGamePlayTags::Player_Ability_Attack_Melee_Light,
			ComboCount);
		NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BaseGamePlayTags::Shared_Event_HitReact, Data);
	}
}

void UPGA_Attack_Melee::UnEquipWeaponFromEvent(FGameplayEventData Data)
{
	UnEquipWeapon();
}

void UPGA_Attack_Melee::EquipWeaponLeftFromEvent(FGameplayEventData Data)
{
	WeaponSocketName = FName("hand_lSocket"); 
	EquipWeapon();
}

void UPGA_Attack_Melee::EquipWeaponRightFromEvent(FGameplayEventData Data)
{
	WeaponSocketName = FName("hand_rSocket"); 
	EquipWeapon();
}
