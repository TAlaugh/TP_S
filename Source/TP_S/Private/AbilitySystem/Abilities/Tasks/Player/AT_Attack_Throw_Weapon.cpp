// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Throw_Weapon.h"

#include "BaseGameplayTags.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "DebugHelper.h"
#include "Items/Weapons/BasePlayerWeapon.h"


UAT_Attack_Throw_Weapon* UAT_Attack_Throw_Weapon::ThrowWeaponToLocation(UGameplayAbility* OwningAbility,
                                                                        AActor* ActorToMove, FVector Destination, float InInterpSpeed)
{
	UAT_Attack_Throw_Weapon* Task = NewAbilityTask<UAT_Attack_Throw_Weapon>(OwningAbility);
	Task->TargetLocation = Destination; // 목표 위치
	Task->Target = ActorToMove;	// 무기
	Task->InterpSpeed = InInterpSpeed; // 속도
	Task->DurationOfMovement = 5.f;
	//Task->bTickingTask = true;
	return Task;
}

void UAT_Attack_Throw_Weapon::Activate()
{
	Super::Activate();
	
	Owner = Cast<ABasePlayerCharacter>(GetAvatarActor());

	Weapon = Owner->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	InterpSpeed = 2.f;
	OwnerLocation = Owner->GetActorLocation();
	if (Weapon)
	{
		TargetLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 2000.f;
		
	}
	
	 
}

void UAT_Attack_Throw_Weapon::TickTask(float DeltaTime)
{
	//Super::TickTask(DeltaTime);

	if (!Target)
	{
		EndTask();
		return;
	}
	OwnerLocation = Owner->GetActorLocation();
	FVector CurrentLocation = Target->GetActorLocation();
	if (!bHasArrived)
	{
		WeaponLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);		
	} else
	{
		WeaponLocation = FMath::VInterpTo(CurrentLocation, OwnerLocation, DeltaTime, InterpSpeed);
	}
	
	Weapon->SetActorLocation(WeaponLocation);

	if (!bHasArrived)
	{
		float Distance = FVector::Dist(WeaponLocation, TargetLocation);
		if (Distance < 10.f) // 근접 도착 판정
		{
			bHasArrived = true;
		}
	} else
	{
		float Distance = FVector::Dist(WeaponLocation, OwnerLocation);
		Debug::Print(TEXT("%f"), Distance);
		if (Distance < 550.f)
		{
			//Owner->GetPlayerCombatComponent()->EquipWeapon(BaseGamePlayTags::Player_Ability_Equip_Melee);
			OnFinished.Broadcast();
			EndTask();
		}
	}
	
}

void UAT_Attack_Throw_Weapon::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}
