// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Light.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

UPGA_Attack_Melee_Light::UPGA_Attack_Melee_Light()
{
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_Light);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Status_WeaponThrown);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Shared_Status_InAir);
	//BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_ReceiveWeapon);
	WeaponSocketName = FName("hand_rSocket");
	AttackType = BaseGamePlayTags::Player_Ability_Attack_Melee_Light;
}

void UPGA_Attack_Melee_Light::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	WeaponSocketName = FName("hand_rSocket");
	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Slide))
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Slide];
		CurrentPlayerState = EPlayerState::Slide;
	}
	else
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Idle];
		CurrentPlayerState = EPlayerState::None;
	}

	if (CurrentPlayerState == EPlayerState::None)
	{
		UAbilityTask_WaitGameplayEvent* TaskToNext = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Event_Attack_Next
		);
		TaskToNext->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::SetNextSection);
		TaskToNext->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* TaskToEquip_Left = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Player_Event_Attack_Equip_Left);
		TaskToEquip_Left->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::EquipWeaponLeftFromEvent);
		TaskToEquip_Left->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* TaskToEquip_Right = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Player_Event_Attack_Equip_Right);
		TaskToEquip_Right->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::EquipWeaponRightFromEvent);
		TaskToEquip_Right->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* TaskToFinish = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Player_Event_Attack_Finish
			);
		TaskToFinish->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Light::StopAttack);
		TaskToFinish->ReadyForActivation();
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPGA_Attack_Melee_Light::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	HasNextComboInput = true;
}

void UPGA_Attack_Melee_Light::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentSection = 0;
	HasNextComboInput = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGA_Attack_Melee_Light::HandleApplyDamage(FGameplayEventData Data)
{
	Super::HandleApplyDamage(Data);

	if (AActor* TargetActor = const_cast<AActor*>(Data.Target.Get()))
	{
		FGameplayCueParameters CueParams(Data.ContextHandle);
		CueParams.Instigator = GetPlayerCharacterFromActorInfo();
		CueParams.EffectCauser = TargetActor;
		CueParams.Location = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->GetSocketLocation(FName("blade"));
		CueParams.TargetAttachComponent = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent();

		K2_ExecuteGameplayCueWithParams(BaseGamePlayTags::GameplayCue_FX_Hit_Melee_PoleArm, CueParams);
	}
}


void UPGA_Attack_Melee_Light::StopAttack(FGameplayEventData Data)
{
	K2_EndAbility();
}
