// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeapons/EnemyBaseWeapon.h"

#include "Character/Enemy/BaseEnemyCharacter.h"

void AEnemyBaseWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	ABaseEnemyCharacter* EnemyCharacter = Cast<ABaseEnemyCharacter>(OtherActor);
	if (EnemyCharacter && WeaponMesh->GetAttachParent() != EnemyCharacter->GetMesh())
	{
		WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponMesh->AttachToComponent(EnemyCharacter->GetMesh(), TransformRules, FName("WeaponSocket"));
	}
	else
	{
		// Handle other actors if necessary
	}
	
}

void AEnemyBaseWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


	
}
