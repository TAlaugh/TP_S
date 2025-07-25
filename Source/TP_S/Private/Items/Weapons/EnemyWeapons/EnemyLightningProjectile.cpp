// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeapons/EnemyLightningProjectile.h"
#include "GameFramework/Pawn.h"
#include "BaseFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"    
#include "Abilities/GameplayAbilityTypes.h"



AEnemyLightningProjectile::AEnemyLightningProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileCollisionBox->OnComponentHit.Clear();
	ProjectileCollisionBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	if (ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->SetWorldScale3D(FVector(1.f, 1.f, 12.f)); // Z축 길게
	}
	// 번개 맞으면 바로 사라지게
	InitialLifeSpan = 1.f;

	// 충돌 정책은 기존과 동일 (Overlap로)
	ProjectileDamagePolicy = EProjectileDamagePolicy::OnBeginOverlap;
}

void AEnemyLightningProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ✅ 0.5초만 데미지 판정 후 충돌 꺼버림
	GetWorldTimerManager().SetTimer(
		CollisionDisableTimer,
		this,
		&AEnemyLightningProjectile::DisableCollision,
		0.7f,  // 판정 유지 시간
		false
	);
}
		
// ✅ Pawn만 처리하는 전용 Overlap
void AEnemyLightningProjectile::OnProjectileBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
		return;

	// ✅ Pawn인지 확인 (플레이어 or AI)
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("⚡ Lightning overlapped NON-Pawn: %s"), *OtherActor->GetName());
		return;
	}

	// ✅ 중복 Overlap 방지
	if (OverlappedActors.Contains(HitPawn))
		return;
	OverlappedActors.AddUnique(HitPawn);

	// ✅ Instigator Pawn 안전 체크
	APawn* InstigatorPawn = GetInstigator<APawn>();
	if (!InstigatorPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ LightningProjectile has NO Instigator! Skipping Hostile check"));
		return;	
	}
	
	// ✅ Hostile 여부 체크 후 데미지
	if (UBaseFunctionLibrary::IsTargetPawnHostile(InstigatorPawn, HitPawn))  // ✅ 여기서 InstigatorPawn 사용
	{
		FGameplayEventData Data;
		Data.Instigator = InstigatorPawn;
		Data.Target = HitPawn;

		HandleApplyProjectileDamage(HitPawn, Data);
	}
}

void AEnemyLightningProjectile::DisableCollision()
{
	UE_LOG(LogTemp, Warning, TEXT("⚡ Lightning collision disabled!"));
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
