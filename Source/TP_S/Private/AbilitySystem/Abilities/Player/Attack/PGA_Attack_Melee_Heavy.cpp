// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/PGA_Attack_Melee_Heavy.h"

#include "BaseGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Throw_Weapon.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UPGA_Attack_Melee_Heavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	EquipWeapon();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ThrowWeapon(FGameplayEventData());
	/*
	UAbilityTask_WaitGameplayEvent* TaskToEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		BaseGamePlayTags::Player_Ability_Attack_Melee_Heavy);
	TaskToEvent->EventReceived.AddDynamic(this, &UPGA_Attack_Melee_Heavy::ThrowWeapon);
	TaskToEvent->ReadyForActivation();
	*/
	
}

void UPGA_Attack_Melee_Heavy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	EquipWeapon();
}

void UPGA_Attack_Melee_Heavy::EquipWeapon()
{
	GetPlayerCombatComponentFromActorInfo()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
}

void UPGA_Attack_Melee_Heavy::UnEquipWeapon(FGameplayEventData TargetData)
{
	GetPlayerCombatComponentFromActorInfo()->UnEquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
}

void UPGA_Attack_Melee_Heavy::ThrowWeapon(FGameplayEventData Data)
{
	ABasePlayerWeapon* Weapon = CachedPlayerCharacter->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	if (Weapon)
	{

		/*
		float InterpSpeed = 500.f;
		FVector TargetLocation = CachedPlayerCharacter->GetActorLocation() + CachedPlayerCharacter->GetActorForwardVector() * 2000.f;
		FVector WeaponLocation = FMath::VInterpTo(CachedPlayerCharacter->GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		Weapon->K2_DetachFromActor();
		//Weapon->SetActorLocation(WeaponLocation);
		UAT_Attack_Throw_Weapon::MoveToLocation(
			this,
			TEXT("Throw"),
			TargetLocation,
			3.f,
			nullptr,
			nullptr
			);
		*/
	}
	
	
	//UAT_Attack_Throw_Weapon TaskToThrow = UAT_Attack_Throw_Weapon();
	//TaskToThrow.Activate();
	
		
}

