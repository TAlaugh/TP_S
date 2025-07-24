// Fill out your copyright notice in the Description page of Project Settings.


//#include "Character/Enemy/BaseEnemyCharacter.h"

#include "TP_S/Public/Character/Enemy/BaseEnemyCharacter.h"

#include "BaseFunctionLibrary.h"
#include "DebugHelper.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Components/Combat/Enemy/EnemyCombatComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "Engine/AssetManager.h"
#include "MotionWarpingComponent.h"
#include "MovieSceneTracksComponentTypes.h"
#include "DataAssets/Enemy/DataAsset_StartupBaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/WidgetBase.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "BaseFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Misc/MapErrors.h"


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
		//UE_LOG(LogTemp, Error, TEXT("BaseAbilitySystemComponent is Exist on %s"), *GetName());
		
	}

	InitEnemyStartUpData();
}

void ABaseEnemyCharacter::PlayLightningEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("⚡ Lightning strike!"));

	// ✅ 번개 이펙트 스폰 (랜덤 위치)
	if (LightningEffect)
	{
		// 번개 위치 랜덤 (보스 주변 2000~4000 범위)
		FVector BossLocation = GetActorLocation();
		FVector RandomOffset = FVector(
			FMath::RandRange(-1500.f, 1500.f),
			FMath::RandRange(-1500.f, 1500.f),
			-200.f // 위쪽에서 떨어지는 느낌
		);

		UNiagaraComponent* LightningComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			LightningEffect,
			BossLocation + RandomOffset,
			FRotator::ZeroRotator,
			FVector(1.0f, 1.0f, 5.0f)
		);
		if (LightningComp)
		{
			LightningComp->SetAutoDestroy(true); // ✅ 끝나면 자동 파괴
		}
	
	}

	if (ThunderSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ThunderSound);
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
		UE_LOG(LogTemp, Warning, TEXT("🌧️ Rain effect spawned"));
	}

	// ✅ 빗소리 재생 (루프)
	if (RainSound)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), RainSound);
		UE_LOG(LogTemp, Warning, TEXT("🎵 Rain sound started"));
	}

	// ✅ 랜덤 번개 + 천둥 타이머 시작
	GetWorldTimerManager().SetTimer(
		LightningTimerHandle,
		this,
		&ABaseEnemyCharacter::PlayLightningEffect,
		FMath::RandRange(3.f, 5.f), 
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