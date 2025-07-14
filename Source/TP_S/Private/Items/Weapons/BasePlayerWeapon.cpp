// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/BasePlayerWeapon.h"

#include "BaseFunctionLibrary.h"

void ABasePlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ABasePlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}

void ABasePlayerWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                  SweepResult);

	APawn* Player = GetInstigator<APawn>();

	checkf(Player, TEXT("Forgot to Assign an Instigator as the Owning Pawn of the Weapon"));

	if (APawn* Enemy = Cast<APawn>(OtherActor))
	{
		if (UBaseFunctionLibrary::IsTargetPawnHostile(Player, Enemy))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void ABasePlayerWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	APawn* Player = GetInstigator<APawn>();

	checkf(Player, TEXT("Forgot to Assign an Instigator as the Owning Pawn of the Weapon"));

	if (APawn* Enemy = Cast<APawn>(OtherActor))
	{
		if (UBaseFunctionLibrary::IsTargetPawnHostile(Player, Enemy))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}
