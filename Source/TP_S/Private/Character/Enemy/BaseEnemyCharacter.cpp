// Fill out your copyright notice in the Description page of Project Settings.


//#include "Character/Enemy/BaseEnemyCharacter.h"

#include "TP_S/Public/Character/Enemy/BaseEnemyCharacter.h"

#include "AIController.h"
#include "BaseFunctionLibrary.h"
#include "BrainComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Components/Combat/Enemy/EnemyCombatComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "Engine/AssetManager.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/WidgetBase.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/EnemyWeapons/EnemyLightningProjectile.h"
#include "Kismet/GameplayStatics.h"

#include "Misc/MapErrors.h"
#include "UObject/FastReferenceCollector.h"


ABaseEnemyCharacter::ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("BaseEnemyCombatComponent"));
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	GetMesh()->SetHiddenInSceneCapture(true);
}

void ABaseEnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}




void ABaseEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UBaseFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!PhaseTwoPostProcessVolume)
	{
		for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
		{
			PhaseTwoPostProcessVolume = *It;
			UE_LOG(LogTemp, Warning, TEXT("✅ Auto-bound PostProcessVolume: %s"), *PhaseTwoPostProcessVolume->GetName());
			break; // 첫 번째 찾은 것만 사용
		}
	}

	if (!BaseAbilitySystemComponent)
		return;

	// ✅ Health Attribute 변화를 감지
	BaseAbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetCurrentHpAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			const float NewHealth = Data.NewValue;
			const float MaxHealth = GetBaseAttributeSet()->GetMaxHp(); 
            
			OnHealthChanged(NewHealth, MaxHealth);
		});
	
	if (!EnemyUIComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyUIComponent is null!"));
		return;
	}
	
	if (BaseAbilitySystemComponent && !BaseAbilitySystemComponent->AbilityActorInfo.IsValid())
	{
		BaseAbilitySystemComponent->InitAbilityActorInfo(this, this); // 백업 Init
	}

	
	if (UWidgetBase* HealthWidget = Cast<UWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreateWidget(this);
	}

	if (BaseAbilitySystemComponent && LightningDamageEffectClass)
	{
		FGameplayEffectContextHandle Ctx = BaseAbilitySystemComponent->MakeEffectContext();
		LightningDamageSpecHandle = BaseAbilitySystemComponent->MakeOutgoingSpec(LightningDamageEffectClass, 1.f, Ctx);
	}
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (BaseAbilitySystemComponent && !BaseAbilitySystemComponent->AbilityActorInfo.IsValid())
	{
		BaseAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	if (BaseAbilitySystemComponent)
	{
		BaseAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		
	}

	InitEnemyStartUpData();
}

void ABaseEnemyCharacter::PlayLightningEffect()
{
    UE_LOG(LogTemp, Warning, TEXT("⚡ Lightning strike!"));

    if (!LightningProjectileClass) return;

    // ✅ 이번 번개 소환 개수 (랜덤 3~5개)
    int32 LightningCount = FMath::RandRange(3, 5);

    for (int32 i = 0; i < LightningCount; i++)
    {
        // 0.3 ~ 0.8초 랜덤 딜레이
        float RandomDelay = FMath::FRandRange(0.3f, 0.8f);

        FTimerHandle TempHandle;
        GetWorldTimerManager().SetTimer(
            TempHandle,
            FTimerDelegate::CreateLambda([this]()
            {
                FVector SpawnLocation = GetRandomLightningLocation();

                // ✅ 경고 이펙트 먼저
                SpawnLightningWarning(SpawnLocation);

                // ✅ 경고 후 실제 번개 딜레이
                float LightningWarningDelay = 1.0f;
                FTimerHandle LightningHandle;
                GetWorldTimerManager().SetTimer(
                    LightningHandle,
                    FTimerDelegate::CreateLambda([this, SpawnLocation]()
                    {
                        SpawnSingleLightning(SpawnLocation);
                    }),
                    LightningWarningDelay,
                    false
                );
            }),
            RandomDelay,
            false
        );
    }
}

	void ABaseEnemyCharacter::SpawnSingleLightning(const FVector& Location)
	{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ World is NULL!"));
		return;
	}

	if (!LightningProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ LightningProjectileClass is NULL!"));
		return;
	}

	AEnemyLightningProjectile* LightningProj = GetWorld()->SpawnActor<AEnemyLightningProjectile>(
		LightningProjectileClass,
		Location,
		FRotator::ZeroRotator
	);

	if (!LightningProj)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Failed to spawn LightningProjectile!"));
		return;
	}

	if (LightningDamageSpecHandle.IsValid())
	{
		LightningProj->ProjectileDamageEffectSpecHandle = LightningDamageSpecHandle;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ LightningDamageSpecHandle is NOT valid!"));
	}

	// Instigator가 이미 파괴 중이면 크래시 방지
	if (!IsPendingKillPending() && IsValid(this))
	{
		LightningProj->SetInstigator(this);
	}
	}

FVector ABaseEnemyCharacter::GetRandomLightningLocation()
{
    FVector FieldCenter = FVector(0.f, 0.f, 0.f);
    FVector RandomOffset = FVector(
        FMath::RandRange(-2500.f, 2500.f),
        FMath::RandRange(-2500.f, 2500.f),
        0.f
    );
    return FieldCenter + RandomOffset;
}

void ABaseEnemyCharacter::SpawnLightningWarning(const FVector& Location)
{
    if (LightningWarningEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            LightningWarningEffect,
            Location,
            FRotator::ZeroRotator,
            FVector(10.f) // 크기
        );
    }

    if (LightningWarningSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightningWarningSound, Location);
    }
}

void ABaseEnemyCharacter::UpdatePhaseTwoDarkness()
{
	if (!PhaseTwoPostProcessVolume) return;

	// ✅ 일정 비율로 증가 (0.05f씩 올림)
	CurrentBlendWeight = FMath::FInterpTo(CurrentBlendWeight, TargetBlendWeight, 0.1f, 0.6f);
	PhaseTwoPostProcessVolume->BlendWeight = CurrentBlendWeight;

	// ✅ 거의 다 어두워지면 타이머 종료
	if (FMath::IsNearlyEqual(CurrentBlendWeight, TargetBlendWeight, 0.02f))
	{
		PhaseTwoPostProcessVolume->BlendWeight = TargetBlendWeight;
		GetWorldTimerManager().ClearTimer(SmoothDarkTimer);
	}
}

void ABaseEnemyCharacter::OnHealthChanged(float NewHealth, float MaxHealth)
{
	if (bIsBoss && !bIsPhaseTwo && NewHealth <= MaxHealth * 0.5f)
	{
		bIsPhaseTwo = true;
		EnterPhaseTwo();
	}
}

	void ABaseEnemyCharacter::EnterPhaseTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("%s entered Phase 2!"), *GetName());

	bIsPhaseTwo = true;

	// AnimInstance 체크
	if (!PhaseTwoMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PhaseTwoMontage is NULL!"));
	}
	if (!GetMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Mesh is NULL!"));
	}
	else if (!GetMesh()->GetAnimInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PhaseTwoMontage cannot be played!"));
		// 몽타주가 없으면 바로 BT 전환
		SwitchToPhaseTwoBehaviorTree();
	}

	// ✅ AI 멈추기
	// ✅ AI 멈추기
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (AICon->BrainComponent)
		{
			AICon->BrainComponent->StopLogic(TEXT("PhaseTwo Montage Playing"));
		}
	}

	// ✅ Phase2 몽타주 실행
	if (PhaseTwoMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		float PlayedLen = GetMesh()->GetAnimInstance()->Montage_Play(PhaseTwoMontage, 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("▶ Phase2 Montage PlayedLen = %f"), PlayedLen);

		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			UE_LOG(LogTemp, Warning, TEXT("▶ Phase2 Montage Finished, Switching BT!"));
			SwitchToPhaseTwoBehaviorTree();
		});
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate, PhaseTwoMontage);
	}
	else
	{
		// 몽타주 없으면 즉시 전환
		SwitchToPhaseTwoBehaviorTree();
	}
	
	if (PhaseTwoPostProcessVolume)
	{
		CurrentBlendWeight = 0.0f; 
		PhaseTwoPostProcessVolume->BlendWeight = 0.0f; // 0 → 1로 부드럽게도 가능

		GetWorldTimerManager().SetTimer(
		   SmoothDarkTimer,
		   this,
		   &ABaseEnemyCharacter::UpdatePhaseTwoDarkness,
		   0.1f,
		   true
		   );
	}
	if (RainEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			RainEffect,
			FVector::ZeroVector, // 월드 전체 비면 0,0,0에 스폰
			FRotator::ZeroRotator
		);
	}

	// ✅ 빗소리 재생 (루프)
	if (RainSound)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), RainSound);
	
	}

	// ✅ 랜덤 번개 + 천둥 타이머 시작
	GetWorldTimerManager().SetTimer(
		LightningTimerHandle,
		this,
		&ABaseEnemyCharacter::PlayLightningEffect,
		FMath::RandRange(1.f, 5.f), 
		true
	);
}

UBaseCombatComponent* ABaseEnemyCharacter::GetBaseCombatComponent() const
{
	return EnemyCombatComponent;
}

UBaseUIComponent* ABaseEnemyCharacter::GetBaseUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* ABaseEnemyCharacter::GetEnemyUIComponent() const
{
	return Cast<UEnemyUIComponent>(EnemyUIComponent);
}

void ABaseEnemyCharacter::InitEnemyStartUpData()
{
	if (StartupData.IsNull())
	{
		return;
	}

	 UAssetManager::GetStreamableManager().RequestAsyncLoad(
	 	StartupData.ToSoftObjectPath(),
	 	FStreamableDelegate::CreateLambda(
	 		[this]()
	 		{
	 			if (UDataAsset_StartupBase* LoadedData = StartupData.Get())
	 			{
					 LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);
				 }
	 		}
	 	)
	);
}

void ABaseEnemyCharacter::SwitchToPhaseTwoBehaviorTree()
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (!AICont)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ No AIController for %s"), *GetName());
		return;
	}

	if (!PhaseTwoBehaviorTree)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PhaseTwoBehaviorTree is NULL for %s"), *GetName());
		return;
	}

	if (AICont->GetBrainComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("🛑 Stopping old BT for %s"), *GetName());
		AICont->GetBrainComponent()->StopLogic(TEXT("Switching to Phase 2"));
	}

	bool bSuccess = AICont->RunBehaviorTree(PhaseTwoBehaviorTree);
	UE_LOG(LogTemp, Warning, TEXT("✅ RunBehaviorTree for PhaseTwo returned: %s"), bSuccess ? TEXT("TRUE") : TEXT("FALSE"));
}

void ABaseEnemyCharacter::Die()
{
	HandleDeath();
}

void ABaseEnemyCharacter::HandleDeath()
{
	OnEnemyDied.Broadcast(this);

	// 기존 죽음 처리 (콜리전 비활성화, Dissolve, Ragdoll 등)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// Dissolve 등 연출 유지 후 파괴
	SetLifeSpan(7.f); // 7초 뒤 Destroy
}
