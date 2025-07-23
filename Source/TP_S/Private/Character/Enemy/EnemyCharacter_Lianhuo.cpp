// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyCharacter_Lianhuo.h"

#include "BaseGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BaseInputComponent.h"
#include "Components/CapsuleComponent.h"

AEnemyCharacter_Lianhuo::AEnemyCharacter_Lianhuo(const FObjectInitializer& ObjectInitializer) : ABaseEnemyCharacter(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.5f, 360.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCapsuleComponent()->SetCapsuleRadius(250.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	
}

UBaseUIComponent* AEnemyCharacter_Lianhuo::GetBaseUIComponent() const
{
	return Cast<UBaseUIComponent>(EnemyUIComponent);
}

UEnemyUIComponent* AEnemyCharacter_Lianhuo::GetEnemyUIComponent() const
{
	return Cast<UEnemyUIComponent>(EnemyUIComponent);
}
