// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Player/PlayerProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "NiagaraComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APlayerProjectileBase::APlayerProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnProjectileBeginOverlap);
	
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	InitialLifeSpan = 1.f;

}

// Called when the game starts or when spawned
void APlayerProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if ( ProjectileDamagePolicy == EPlayerProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	}
}

void APlayerProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	
	APawn* HitPawn = Cast<APawn>(OtherActor);


	if (!HitPawn || !UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		Destroy();
		return;
	}
		
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	
	HandleApplyProjectileDamage(HitPawn, Data);
	
	
	Destroy();
	
}

void APlayerProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		return;
	}
	OverlappedActors.AddUnique(OtherActor);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		FGameplayEventData Data;
		Data.Instigator = GetInstigator();
		Data.Target = HitPawn;
		
		if (UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
		{
			HandleApplyProjectileDamage(HitPawn, Data);
		}
	}
}

void APlayerProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(),TEXT("Forget to assign a valid spec handle to the projectile: %s"),*GetActorNameOrLabel())
	const bool bWasApplied =  UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToActor(GetInstigator(),InHitPawn,ProjectileDamageEffectSpecHandle);
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn,
			BaseGamePlayTags::Shared_Event_HitReact,
			InPayload
			);
	}
}

void APlayerProjectileBase::BP_HandleApplyProjectileDamage()
{
	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (APawn* HitPawn = Cast<APawn>(OverlappedActor))
		{
			FGameplayEventData Data;
			Data.Instigator = GetInstigator();
			Data.Target = HitPawn;

			if (UBaseFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
			{
				HandleApplyProjectileDamage(HitPawn, Data);
			}
		}
	}
}


