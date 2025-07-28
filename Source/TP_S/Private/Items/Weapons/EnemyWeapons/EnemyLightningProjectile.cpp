// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EnemyWeapons/EnemyLightningProjectile.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"         // ✅ FOverlapResult 포함
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "BaseFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	
	GetWorldTimerManager().SetTimerForNextTick(this, &AEnemyLightningProjectile::TryApplyInitialOverlapDamage);
	
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
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!IsValid(HitPawn) || HitPawn || this || HitPawn->IsActorBeingDestroyed())
		return;

	// ✅ Instigator 체크
	APawn* InstigatorPawn = GetInstigator<APawn>();
	if (!IsValid(InstigatorPawn) || InstigatorPawn->IsActorBeingDestroyed())
		return;
	
	if (OverlappedActors.Contains(HitPawn))
		return;
	OverlappedActors.AddUnique(HitPawn);


	if (!UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator<APawn>(), HitPawn))
		return;

		FGameplayEventData Data;
		Data.Instigator = InstigatorPawn;
		Data.Target = HitPawn;

		HandleApplyProjectileDamage(HitPawn, Data);
	
}

void AEnemyLightningProjectile::DisableCollision()
{
	UE_LOG(LogTemp, Warning, TEXT("⚡ Lightning collision disabled!"));
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FGenericTeamId AEnemyLightningProjectile::GetGenericTeamId() const
{
	if (APawn* InstigatorPawn = GetInstigator<APawn>())
	{
		if (const IGenericTeamAgentInterface* InstigatorTeamAgent = Cast<IGenericTeamAgentInterface>(InstigatorPawn->GetController()))
		{
			return InstigatorTeamAgent->GetGenericTeamId();
		}
	}
	// 기본적으로 중립 (팀 없음)
	return FGenericTeamId::NoTeam;
}

void AEnemyLightningProjectile::TryApplyInitialOverlapDamage()
{
	TArray<AActor*> HitActors;

	UKismetSystemLibrary::BoxOverlapActors(
		GetWorld(),
		ProjectileCollisionBox->GetComponentLocation(),
		ProjectileCollisionBox->GetScaledBoxExtent(),
		TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_Pawn)},
		APawn::StaticClass(),
		TArray<AActor*>{this},
		HitActors
	);

	for (AActor* Actor : HitActors)
	{
		APawn* HitPawn = Cast<APawn>(Actor);
		if (!IsValid(HitPawn) || HitPawn->IsActorBeingDestroyed())
			continue;

		if (OverlappedActors.Contains(HitPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("❗ %s 는 이미 OverlappedActors에 있음 → 스킵"), *HitPawn->GetName());
			continue;
		}

		APawn* InstigatorPawn = GetInstigator<APawn>();
		if (!IsValid(InstigatorPawn))
			continue;

		if (!UBaseFunctionLibrary::IsTargetPawnHostile(InstigatorPawn, HitPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("🤝 %s 은 적이 아님 → 스킵"), *HitPawn->GetName());
			continue;
		}

		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPawn);
		if (!ASC)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ ASC가 없음: %s"), *HitPawn->GetName());
			continue;
		}
		if (!ASC->AbilityActorInfo.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("❌ ASC 초기화 안됨: %s"), *HitPawn->GetName());
			continue;
		}

		if (!ProjectileDamageEffectSpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("❌ SpecHandle 무효"));
			continue;
		}

		OverlappedActors.Add(HitPawn);

		FGameplayEventData Data;
		Data.Instigator = InstigatorPawn;
		Data.Target = HitPawn;

		UE_LOG(LogTemp, Warning, TEXT("✅ 데미지 적용 시도: %s"), *HitPawn->GetName());
		HandleApplyProjectileDamage(HitPawn, Data);
	}
}