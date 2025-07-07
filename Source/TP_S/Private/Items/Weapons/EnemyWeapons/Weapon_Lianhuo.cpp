// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeapons/Weapon_Lianhuo.h"

#include "Character/Enemy/EnemyCharacter_Lianhuo.h"

AWeapon_Lianhuo::AWeapon_Lianhuo()
{
	GetWeaponCollisionBox()->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(140.f, 0.f, -30.f), FVector(15.f, 1.f, 2.f)));
}

void AWeapon_Lianhuo::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                  SweepResult);

	AEnemyCharacter_Lianhuo* EnemyCharacter = Cast<AEnemyCharacter_Lianhuo>(OtherActor);
	if (EnemyCharacter)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponMesh->AttachToComponent(EnemyCharacter->GetMesh(), TransformRules, FName("WeaponSocket"));
	}
	else
	{
		// Handle other actors if necessary
	}
}

void AWeapon_Lianhuo::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
