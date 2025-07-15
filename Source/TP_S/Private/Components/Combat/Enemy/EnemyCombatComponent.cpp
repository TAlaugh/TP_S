// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Enemy/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/GameplayAbilityTypes.h"
#include  "Items/Weapons/BaseWeapon.h"

void UEnemyCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABaseWeapon* Weapon,
	bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(WeaponTag), TEXT("%s has already been as Carried Weapon"), *WeaponTag.ToString());
	check(Weapon);

	CharacterCarriedWeaponMap.Emplace(WeaponTag, Weapon);

	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this,&ThisClass::OnWeaponPulledFromTargetActor);

	//장착한 무기로 등록이 되면 현재 장착무기를 변경
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;

		UE_LOG(LogTemp,Warning,TEXT("Has been registered using the tag : %s"), *WeaponTag.ToString());
	}
}

ABaseWeapon* UEnemyCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(WeaponTag))
	{
		if (ABaseWeapon* const* FoundWeapon = CharacterCarriedWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

ABaseWeapon* UEnemyCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UEnemyCombatComponent::ToggleWeaponCollision(bool bUse, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ABaseWeapon* Weapon = GetCharacterCurrentEquippedWeapon();
		check(Weapon);
		
		if (bUse)
		{
			OverlappedActors.Empty();
			Weapon->SetCollisionActive(false);
			
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			UE_LOG(LogTemp, Warning, TEXT("Collision Enabled"));
		}
		
		else
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT(" Collision Disabled"));
			
		}

		
	}

	
}

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	
	if (!HitActor)
    	{
    		UE_LOG(LogTemp, Error, TEXT("HitActor is null in OnHitTargetActor!"));
    		return;
    	}

	if (OverlappedActors.Contains(HitActor))
	{
		
		return;
	}

	
	OverlappedActors.AddUnique(HitActor);
	
 	////TODO:: Implement Block Check
	////블락 판단유무
	//bool bIsValidBlock = false;
	
	//플레이어 블락중인지
	//const bool bIsPlayerBlocking = UBaseFunctionLibrary::NativeActorHasTag(HitActor, BaseGamePlayTags::Player_Status_Blocking);

	//블락이 불가능한 상태인지 (무적기)
	// const bool bIsAttackUnBlockable = false;
	//
	// if (bIsPlayerBlocking && !bIsAttackUnBlockable)
	// {
	// 	bIsValidBlock = UBaseFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	// }

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	// if (bIsValidBlock)
	// {
	// 	//피격자에게 게임플레이이벤트 전송 GA_Hero_Block
	// 	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, BaseGamePlayTags::Player_Event_SuccessBlock,EventData);
	// 	
	// }
	// else
	// {
	// 	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_MeleeHit, EventData);
	// }

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_Hit_Melee, EventData);
	
}

void UEnemyCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
	if (InteractedActor)
	{
		OverlappedActors.Remove(InteractedActor);  // 겹침 해제되면 리스트에서 제거
	}
}
