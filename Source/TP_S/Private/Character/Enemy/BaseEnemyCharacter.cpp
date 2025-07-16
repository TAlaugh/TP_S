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
#include "BaseFunctionLibrary.h"

#include "Misc/MapErrors.h"


ABaseEnemyCharacter::ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 270.0f, 0.0f);
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
	else
	{
		Debug::Print("e");
	}

	InitEnemyStartUpData();
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
					 Debug::Print(TEXT("Enemy Startup Data Loaded"), FColor::Green);
				 }
	 		}
	 	)
	);
}
