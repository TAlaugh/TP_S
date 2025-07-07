// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Throw_Weapon.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "DebugHelper.h"
#include "Items/Weapons/BasePlayerWeapon.h"

void UAT_Attack_Throw_Weapon::Activate()
{
	Super::Activate();

	Owner = Cast<ABasePlayerCharacter>(GetAvatarActor());

	Weapon = Owner->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
	InterpSpeed = 500.f;

	if (Weapon)
	{
		TargetLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 1000.f;
		
	}
	 
}

void UAT_Attack_Throw_Weapon::TickTask(float DeltaTime)
{
	//Super::TickTask(DeltaTime);

	if (bIsFinished)
	{
		return;
	}
	Debug::Print(TEXT("%f"), DeltaTime);
	WeaponLocation = FMath::VInterpTo(Owner->GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	Weapon->SetActorLocation(WeaponLocation);
}
