// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ThrowWeapon.h"

#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"

UAT_Attack_Melee_ThrowWeapon* UAT_Attack_Melee_ThrowWeapon::Init(UGameplayAbility* OwningAbility,
                                                                   FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UAT_Attack_Melee_ThrowWeapon* Task = NewAbilityTask<UAT_Attack_Melee_ThrowWeapon>(OwningAbility);
	Task->Tag = EventTag;
	Task->OnlyTriggerOnce = OnlyTriggerOnce;
	Task->OnlyMatchExact = OnlyMatchExact;
	Task->bTickingTask = true;
	return Task;
}

void UAT_Attack_Melee_ThrowWeapon::Activate()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
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
				MyHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UAT_Attack_Melee_ThrowWeapon::GameplayEventCallback);
			}
			
		}
	}
}

void UAT_Attack_Melee_ThrowWeapon::GameplayEventContainerCallback(FGameplayTag MatchingTag,
	const FGameplayEventData* Payload)
{
	bHasThrow = true;
	Weapon->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative);
	
	// 장착중인 무기를 던졌을 경우 태그랑 위치를 컴뱃 컴포넌트에 저장
	Player->GetPlayerCombatComponent()->WeaponThrownLocation = TargetLocation;
	Player->GetPlayerCombatComponent()->CurrentThrownWeaponTag = Player->GetPlayerCombatComponent()->CurrentEquippedWeaponTag;
	Player->GetPlayerCombatComponent()->ToggleWeaponCollision(true);
	
	Weapon->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(Weapon->ItemMontage);
}

void UAT_Attack_Melee_ThrowWeapon::TickTask(float DeltaTime)
{
	if (bHasThrow)
	{
		FVector CurrentLocation = Weapon->GetActorLocation();
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
						EndTask();
					}),
					1.5f,
					false);
			}
		}
	}
}