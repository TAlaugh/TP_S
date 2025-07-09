// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Melee_Attack_Throw_Weapon.h"

#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

UAT_Melee_Attack_Throw_Weapon* UAT_Melee_Attack_Throw_Weapon::Init(UGameplayAbility* OwningAbility,
                                                                   FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UAT_Melee_Attack_Throw_Weapon* Task = NewAbilityTask<UAT_Melee_Attack_Throw_Weapon>(OwningAbility);
	Task->Tag = EventTag;
	Task->OnlyTriggerOnce = OnlyTriggerOnce;
	Task->OnlyMatchExact = OnlyMatchExact;
	Task->bTickingTask = true;
	return Task;
}

void UAT_Melee_Attack_Throw_Weapon::Activate()
{
	Timer.Invalidate();
	Player = Cast<ABasePlayerCharacter>(GetAvatarActor());
	if (Player)
	{
		Weapon = Player->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
		PlayerLocation = Player->GetActorLocation();
		if (Weapon)
		{
			TargetLocation = PlayerLocation + Player->GetActorForwardVector() * 2000.f;
			UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();
			if (Ability && ASC)
			{
				MyHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UAT_Melee_Attack_Throw_Weapon::GameplayEventCallback);
			}
			
		}
	}
}

void UAT_Melee_Attack_Throw_Weapon::GameplayEventContainerCallback(FGameplayTag MatchingTag,
	const FGameplayEventData* Payload)
{
	bHasThrow = true;
	Weapon->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative);

	Weapon->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(Weapon->ItemMontage);
	// EventReceived 구현부
	if (Player && Weapon)
	{
		/*
		if (!Timer.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(
				Timer,
				FTimerDelegate::CreateLambda([this]()
				{
					
				}),
				3.f,
				false);
		}
		
		Timer.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(
			Timer,
			FTimerDelegate::CreateLambda([this]()
			{
				FVector CurrentLocation = Weapon->GetActorLocation();
				PlayerLocation = Player->GetActorLocation();
				WeaponLocation = FMath::VInterpTo(CurrentLocation, PlayerLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
				Weapon->SetActorLocation(WeaponLocation);

				if (float Distance = FVector::Dist(CurrentLocation, PlayerLocation) <= 100.f)
				{
					Player->GetPlayerCombatComponent()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
					EndTask();
					return;
				}
			}),
			3.f,
			false
			);
		*/
	}
}

void UAT_Melee_Attack_Throw_Weapon::TickTask(float DeltaTime)
{
	FVector CurrentLocation = Weapon->GetActorLocation();
	FRotator CurrentRotation = Weapon->GetActorRotation();
	CurrentRotation.Yaw += 360.f * DeltaTime;
	//Weapon->SetActorRotation(CurrentRotation);
	if (bHasThrow)
	{
		if (!bHasArrived)
		{
			WeaponLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
			Weapon->SetActorLocation(WeaponLocation);
			if (float Distance = FVector::Dist(CurrentLocation, WeaponLocation) <= 10.f)
			{
				
				if (!Timer.IsValid())
				{
					GetWorld()->GetTimerManager().SetTimer(
						Timer,
						FTimerDelegate::CreateLambda([this]()
						{
							bHasArrived = true;
							TargetLocation = PlayerLocation;
						}),
						1.5f,
						false);
				}
			}
		}
		else
		{
			WeaponLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
			Weapon->SetActorLocation(WeaponLocation);
			if (float Distance = FVector::Dist(CurrentLocation, TargetLocation) <= 300.f)
			{
				Player->GetPlayerCombatComponent()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
				Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Stop(0.f);
				EndTask();
				return;
			}
		}
	}
}