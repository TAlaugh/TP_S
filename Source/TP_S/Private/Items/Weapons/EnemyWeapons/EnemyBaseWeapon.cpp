// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeapons/EnemyBaseWeapon.h"

#include "BaseFunctionLibrary.h"
#include "Character/Enemy/BaseEnemyCharacter.h"

void AEnemyBaseWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	ABaseEnemyCharacter* EnemyCharacter = Cast<ABaseEnemyCharacter>(OtherActor);
	AActor* MyOwner = GetOwner();
	//
	// if (!EnemyCharacter || EnemyCharacter != MyOwner)
	// {
	// 	return; // ❗ 다른 몬스터면 무시
	// }
	//
	// 1. 자기 자신은 무시
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn)
		return;

	// 타겟이 적인지 판단 (예: 팀 비교, 태그, 함수 등)
	if (!UBaseFunctionLibrary::IsTargetPawnHostile(Cast<APawn>(MyOwner), HitPawn))
	{
		return;
	}

	// 로그로 확인
	UE_LOG(LogTemp, Warning, TEXT("✅ Weapon Hit Target: %s"), *GetNameSafe(HitPawn));

	// 피격 처리 이벤트 전달
	if (OnWeaponHitTarget.IsBound())
	{
		OnWeaponHitTarget.Execute(HitPawn);
	}

	// (선택) 무기 다시 Attach 로직 (Owner 확인 기반으로)
	if (ABaseEnemyCharacter* EnemyCharacterOwner = Cast<ABaseEnemyCharacter>(MyOwner))
	{
		if (WeaponMesh->GetAttachParent() != EnemyCharacterOwner->GetMesh())
		{
			WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
			FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
			WeaponMesh->AttachToComponent(EnemyCharacterOwner->GetMesh(), TransformRules, FName("WeaponSocket"));
		}
	}
}


void AEnemyBaseWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


	
}
