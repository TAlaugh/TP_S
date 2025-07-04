// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "AbilitySystem/Player/PlayerAbilitySystemComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UBasePlayerCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon,
                                                       bool bRegisterAsEquippedWeapon)
{
	checkf(!PlayerCarriedWeaponMap.Contains(WeaponTag), TEXT("Already Equipped"), *WeaponTag.ToString());

	check(Weapon);

	PlayerCarriedWeaponMap.Emplace(WeaponTag, Weapon);
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	// 장착한 무기로 등록이 되면 현재 장착 무기를 변경
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
	}
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (PlayerCarriedWeaponMap.Contains(WeaponTag))
	{
		if (ABasePlayerWeapon* const* FoundWeapon = PlayerCarriedWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetPlayerCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UBasePlayerCombatComponent::EquipWeapon()
{
	if (!GetPlayerCurrentEquippedWeapon())
	{
		//return;
	}
	Debug::Print("Equip");
	GetPlayerCurrentEquippedWeapon()->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("hand_rSocket"));
}

void UBasePlayerCombatComponent::UnEquipWeapon()
{
	Debug::Print("UnEquip");
	GetPlayerCurrentEquippedWeapon()->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("hook_1_back_weapon"));
}

float UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const
{
	//return GetPlayerCurrentEquippedWeapon()->data.WeaponBaseDamage.GetValueAtLevel(Level);
	return Level;
}

void UBasePlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_HitReact, Data);
}

void UBasePlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}

void UBasePlayerCombatComponent::ToggleWeaponCollision(bool bUse, EPlayerToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EPlayerToggleDamageType::CurrentEquippedWeapon)
	{
		ABasePlayerWeapon* Weapon = GetPlayerCurrentEquippedWeapon();

		check(Weapon);

		if (bUse)
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
}
