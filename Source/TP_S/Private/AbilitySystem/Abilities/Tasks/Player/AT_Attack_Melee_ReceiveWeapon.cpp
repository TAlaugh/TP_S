// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ReceiveWeapon.h"

#include "BaseFunctionLibrary.h"
#include "DebugHelper.h"
#include "BaseGameplayTags.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"
#include "Kismet/KismetMathLibrary.h"

UAT_Attack_Melee_ReceiveWeapon* UAT_Attack_Melee_ReceiveWeapon::Init(UGameplayAbility* OwningAbility)
{
	UAT_Attack_Melee_ReceiveWeapon* Task = NewAbilityTask<UAT_Attack_Melee_ReceiveWeapon>(OwningAbility);
	Task->bTickingTask = true;

	return Task;
}

void UAT_Attack_Melee_ReceiveWeapon::Activate()
{
	Player = Cast<ABasePlayerCharacter>(GetAvatarActor());
	if (Player)
	{
		FGameplayTag WeaponTag = Player->GetPlayerCombatComponent()->CurrentThrownWeaponTag; 
		Weapon = Player->GetPlayerCombatComponent()->GetPlayerCarriedWeaponByTag(WeaponTag);
		PlayerLocation = Player->GetActorLocation();
		PlayerRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, Player->GetPlayerCombatComponent()->WeaponThrownLocation);
		Player->SetActorRotation(PlayerRotation);
	}
	Super::Activate();
}

void UAT_Attack_Melee_ReceiveWeapon::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (Weapon)
	{
		FVector CurrentLocation = Weapon->GetActorLocation();
		WeaponLocation = FMath::VInterpTo(CurrentLocation, PlayerLocation, DeltaTime, InterpSpeed);
		Weapon->SetActorLocation(WeaponLocation);
		// 플레이어에게 가까이 왔을 때 잡기처리
		if (FVector::Dist(CurrentLocation, PlayerLocation) <= 300.f)
		{
			Player->GetPlayerCombatComponent()->EquipWeapon(BaseGamePlayTags::Item_Equipable_Weapon_Melee);
			Player->GetPlayerCombatComponent()->ToggleWeaponCollision(false);
			Player->GetPlayerCombatComponent()->WeaponThrownLocation = FVector();
			Player->GetPlayerCombatComponent()->CurrentThrownWeaponTag = FGameplayTag();
			if (Weapon->GetSkeletalMeshComponent()->GetAnimInstance())
			{
				Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Stop(0.f);
			}
			UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(Player, BaseGamePlayTags::Player_Status_WeaponThrown);
			OnReceivedDelegate.Broadcast();
			EndTask();
			return;
		}
	}
}
