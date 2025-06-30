// Fill out your copyright notice in the Description page of Project Settings.


#include "Chacracter/Enemy/BaseEnemyCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Components/Combat/Enemy/EnemyCombatComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/WidgetBase.h"


ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;

	BaseEnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("BaseEnemyCombatComponent"));
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
}



void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartUpData();
}

UBaseCombatComponent* ABaseEnemyCharacter::GetBaseCombatComponent() const
{
	return BaseEnemyCombatComponent;
}

UBaseUIComponent* ABaseEnemyCharacter::GetBaseUIComponent() const
{
	return EnemyUIComponent;
}

UBaseUIComponent* ABaseEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
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
