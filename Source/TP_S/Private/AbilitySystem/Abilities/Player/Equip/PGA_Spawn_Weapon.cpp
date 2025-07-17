// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Equip/PGA_Spawn_Weapon.h"

#include "DebugHelper.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Items/Weapons/BasePlayerWeapon.h"
#include "Items/Weapons/WeaponItemDataAsset.h"

UPGA_Spawn_Weapon::UPGA_Spawn_Weapon()
{
	AbilityActivationPolicy = EbaseAbilityActivationPolicy::OnTriggered;
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_SpawnWeapon);
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerTag = BaseGamePlayTags::Player_Event_Equip;
	AbilityTriggers.Add(TriggerData);
}

void UPGA_Spawn_Weapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	const UClass* EventClass = Cast<UClass>(TriggerEventData->OptionalObject.Get());
	if (EventClass && EventClass->IsChildOf(ABaseWeapon::StaticClass()))
	{
		WeaponClass = const_cast<UClass*>(EventClass);
	}
	if (WeaponClass == nullptr)
	{
		K2_CancelAbility();
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetAvatarActorFromActorInfo();
	SpawnParameters.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	UWorld* World = GetAvatarActorFromActorInfo()->GetWorld();

	FVector SpawnLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FRotator SpawnRotation = GetPlayerCharacterFromActorInfo()->GetActorRotation();
	
	Weapon = Cast<ABasePlayerWeapon>(World->SpawnActor(WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParameters));
	if (Weapon)
	{
		FAttachmentTransformRules Attachment = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepWorld,
			true);
		
		// 태그를 매치해서 근접무기인지 원거리 무기인지 판별하여 Register
		if (Weapon->ItemDataAsset)
		{
			FGameplayTag WeaponType = Weapon->ItemDataAsset->GetWeaponGameplayTag().MatchesTag(BaseGamePlayTags::Item_Equipable_Weapon_Melee) ? BaseGamePlayTags::Item_Equipable_Weapon_Melee : BaseGamePlayTags::Item_Equipable_Weapon_Range;
			SocketNameToAttachment = WeaponType == BaseGamePlayTags::Item_Equipable_Weapon_Melee ? FName("hook_1_back_weaponSocket") : FName("hook_2_back_weaponSocket");
			Weapon->AttachToComponent(GetPlayerCharacterFromActorInfo()->GetMesh(), Attachment, SocketNameToAttachment);

			// 기존 무기 해제
			if (ABasePlayerWeapon* CurrentPlayerWeapon = GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->GetPlayerCarriedWeaponByTypeTag(WeaponType))
			{
				FGameplayTag CurrentPlayerWeaponTag = CurrentPlayerWeapon->ItemDataAsset->GetWeaponGameplayTag();
				GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->RemoveSpawnedWeapon(CurrentPlayerWeaponTag, CurrentPlayerWeapon, WeaponType);
			}
			// 새로운 무기 장착
			GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent()->RegisterSpawnedWeapon(Weapon->ItemDataAsset->GetWeaponGameplayTag(), Weapon, WeaponType);
		}
	}
	K2_EndAbility();
}
