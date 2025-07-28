// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/EnemyWeapons/EnemyProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "EnemyLightningProjectile.generated.h"

class UNiagaraSystem;
class UBoxComponent;

/**
 *
 * 
 */
UCLASS()
class TP_S_API AEnemyLightningProjectile : public AEnemyProjectileBase
{
	GENERATED_BODY()
	
public:
	AEnemyLightningProjectile();
	virtual void BeginPlay()override;
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult) override;
	void DisableCollision();
	
	virtual FGenericTeamId GetGenericTeamId() const;
	void TryApplyInitialOverlapDamage();

	UPROPERTY(EditDefaultsOnly, Category="Lightning")
	UNiagaraSystem* WarningNiagara;

private:
	// ✅ 타이머 핸들
	FTimerHandle CollisionDisableTimer;
};