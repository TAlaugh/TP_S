// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "AbilitySystem/Player/PlayerAbilitySystemComponent.h"
#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UBasePlayerCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon, FGameplayTag WeaponType)
{
	checkf(!PlayerWeaponMap.Contains(WeaponTag), TEXT("Already Equipped"), *WeaponTag.ToString());

	check(Weapon);

	PlayerWeaponMap.Emplace(WeaponTag, Weapon);
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (WeaponType == BaseGamePlayTags::Player_Ability_Equip_Melee){
		CurrentEquippedMeleeWeaponTag = WeaponTag;
	} else
	{
		CurrentEquippedRangeWeaponTag = WeaponTag;
	}
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (PlayerWeaponMap.Contains(WeaponTag))
	{
		if (ABasePlayerWeapon* const* FoundWeapon = PlayerWeaponMap.Find(WeaponTag))
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

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponByTag(FGameplayTag WeaponType) const
{
	if (WeaponType == BaseGamePlayTags::Player_Ability_Equip_Melee && !CurrentEquippedMeleeWeaponTag.IsValid())
	{
		return nullptr;
	}

	if (WeaponType == BaseGamePlayTags::Player_Ability_Equip_Range && !CurrentEquippedRangeWeaponTag.IsValid())
	{
		return nullptr;
	}
	
	return GetPlayerCarriedWeaponByTag(WeaponType == BaseGamePlayTags::Player_Ability_Equip_Melee ? CurrentEquippedMeleeWeaponTag : CurrentEquippedRangeWeaponTag);
}

void UBasePlayerCombatComponent::EquipWeapon(FGameplayTag WeaponType, FName SocketName)
{
	if (!GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		return;
	}
	
	GetPlayerCurrentEquippedWeaponByTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	if (WeaponType == BaseGamePlayTags::Player_Ability_Equip_Melee)
	{
		CurrentEquippedWeaponTag = CurrentEquippedMeleeWeaponTag;
	}
	else
	{
		CurrentEquippedWeaponTag = CurrentEquippedRangeWeaponTag;
		GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->SetVisibility(true);
	}
}

void UBasePlayerCombatComponent::UnEquipWeapon(FGameplayTag WeaponType)
{
	if (!GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		return;
	}
	
	FName SocketName;
	if (WeaponType == BaseGamePlayTags::Player_Ability_Equip_Melee)
	{
		SocketName = MeleeSocketName;
	} else
	{
		GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->SetVisibility(false);
		SocketName = RangeSocketName;
	}
	
	GetPlayerCurrentEquippedWeaponByTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	CurrentEquippedWeaponTag = FGameplayTag();
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

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_Hit, Data);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_HitReact, FGameplayEventData());
}

void UBasePlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}

void UBasePlayerCombatComponent::ToggleWeaponCollision(bool bUse, EPlayerToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EPlayerToggleDamageType::CurrentEquippedWeapon)
	{
		ABasePlayerWeapon* Weapon = GetPlayerCurrentEquippedWeapon();

		if (Weapon)
		{
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
	
}

