// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"

#include <string>

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Player/PlayerAttributeSet.h"
#include "BaseType/BaseEnumType.h"
#include "Camera/CameraComponent.h"
#include "Components/BaseInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "DataAssets/Player/DataAsset_StartupBasePlayer.h"
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

	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UBasePlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	PlayerAbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("PlayerAbilitySystemComponent"));
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
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
	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ABasePlayerCharacter::Input_AbilityInputPressed, &ABasePlayerCharacter::Input_AbilityInputTriggered, &ABasePlayerCharacter::Input_AbilityInputReleased);
	
	PlayerAbilitySystemComponent->BindAbilityActivationToInputComponent(BaseInputComponent,
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


}

void ABasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (PlayerAbilitySystemComponent)
	{
		PlayerAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartupBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			PlayerStartUpData = Cast<UDataAsset_StartupBasePlayer>(LoadedData);
			PlayerStartUpData->GiveToAbilitySystemComponent(PlayerAbilitySystemComponent);
		}
	}

	ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forget to assigned Startup data to : %s"), *GetName());
}

UAbilitySystemComponent* ABasePlayerCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerAbilitySystemComponent();
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
	PlayerAbilitySystemComponent->OnAbilityInputPressed(InputTag);
	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = true;
	}
}

void ABasePlayerCharacter::Input_AbilityInputTriggered(const FGameplayTag InputTag)
{
	PlayerAbilitySystemComponent->OnAbilityInputTriggered(InputTag);
	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = false;
	}
}

void ABasePlayerCharacter::Input_AbilityInputReleased(const FGameplayTag InputTag)
{
	PlayerAbilitySystemComponent->OnAbilityInputReleased(InputTag);
	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = false;
	}
}
