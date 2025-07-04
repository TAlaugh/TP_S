// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"

#include <string>

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseType/BaseEnumType.h"
#include "Camera/CameraComponent.h"
#include "Components/BaseInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/BaseQuickSlotComponent.h"
#include "Components/Inventory/ConsumableInventoryComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.5f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	ABasePlayerCharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	ABasePlayerCharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	ABasePlayerCharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanWalk = true;

	ConsumableInventoryComponent = CreateDefaultSubobject<UConsumableInventoryComponent>(TEXT("ConsumableInventory"));
	QuickSlotComponent = CreateDefaultSubobject<UBaseQuickSlotComponent>(TEXT("QuickSlot"));

}

void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as Input Config"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem =  ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);

	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGamePlayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ABasePlayerCharacter::Input_AbilityInputPressed, &ABasePlayerCharacter::Input_AbilityInputReleased);
	
	BaseAbilitySystemComponent->BindAbilityActivationToInputComponent(BaseInputComponent,
		FGameplayAbilityInputBinds("Confirm", "Cancel", FTopLevelAssetPath(TEXT("/Script/TP_S.EAbility")),
			static_cast<int32>(EAbility::Confirm), static_cast<int32>(EAbility::Cancel)));

}

void ABasePlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpCount = 0;
	
	/*
	FGameplayEventData Data;
	Data.EventTag = BaseGamePlayTags::Shared_Event_Land;
	Data.Instigator = this;
	Data.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Data.EventTag, Data);
	*/
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (QuickSlotComponent && ConsumableInventoryComponent)
	{
		QuickSlotComponent->Initialize(ConsumableInventoryComponent);
	}
}

void ABasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartupBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);
		}
	}

	ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forget to assigned Startup data to : %s"), *GetName());
}

void ABasePlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f,Controller->GetControlRotation().Yaw,0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection,MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection,MovementVector.X);
	}
}

void ABasePlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABasePlayerCharacter::Input_AbilityInputPressed(const FGameplayTag InputTag)
{
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);

	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = true;
	}
}

void ABasePlayerCharacter::Input_AbilityInputReleased(const FGameplayTag InputTag)
{
	BaseAbilitySystemComponent->OnAbilityInputReleased(InputTag);

	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = false;
	}
}
