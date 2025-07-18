// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/BasePlayerWeapon.h"

#include "BaseFunctionLibrary.h"
#include "DebugHelper.h"

ABasePlayerWeapon::ABasePlayerWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasePlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponCollisionBox->AttachToComponent(
		WeaponMesh,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false),
		CollisionSocketName);
	SetActorTickEnabled(false);
}

void ABasePlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDissolving)
	{
		DissolveTimeElapsed += DeltaTime;
		float DissolveAlpha = FMath::Clamp(DissolveTimeElapsed / DissolveDuration, 0.f, 1.f);
		WeaponMesh->SetScalarParameterValueOnMaterials(FName("Dissolve Amount"), 1 + DissolveAlpha);

		if (DissolveAlpha >= 1.2f)
		{
			bIsDissolving = false;
			SetActorTickEnabled(false);
		}
	}
}

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

void ABasePlayerWeapon::DissolveFXWeapon(bool bDissolve)
{
	if (WeaponMesh && bDissolve)
	{
		bIsDissolving = true;
		DissolveTimeElapsed = 0.f;
		WeaponMesh->SetScalarParameterValueOnMaterials(FName("Dissolve Amount"),1);
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
		WeaponMesh->SetScalarParameterValueOnMaterials(FName("Dissolve Amount"),3);
	}
}
