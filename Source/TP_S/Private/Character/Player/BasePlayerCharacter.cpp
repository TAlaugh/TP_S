// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"


#include "AbilitySystemBlueprintLibrary.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BaseInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "Components/Movement/PlayerMovementComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "DataAssets/Player/DataAsset_StartupBasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerState/BasePlayerState.h"

ABasePlayerCharacter::ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
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

	UPlayerMovementComponent* PlayerMoveComp = CastChecked<UPlayerMovementComponent>(GetCharacterMovement());
	PlayerMoveComp->GravityScale = 1.0f;
	PlayerMoveComp->MaxAcceleration = 2400.0f;
	PlayerMoveComp->BrakingFrictionFactor = 1.0f;
	PlayerMoveComp->BrakingFriction = 6.0f;
	PlayerMoveComp->GroundFriction = 8.0f;
	//PlayerMoveComp->BrakingDecelerationWalking = 1400.0f;
	PlayerMoveComp->bUseControllerDesiredRotation = false;
	PlayerMoveComp->bOrientRotationToMovement = true;
	PlayerMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	PlayerMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanJump = true;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanWalk = true;
	PlayerMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	//PlayerMoveComp->SetCrouchedHalfHeight(95.0f);
	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
	PlayerQuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UBasePlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	//PlayerAbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("PlayerAbilitySystemComponent"));
	//PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
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
	
	BaseAbilitySystemComponent->BindAbilityActivationToInputComponent(BaseInputComponent,
		FGameplayAbilityInputBinds("Confirm", "Cancel", FTopLevelAssetPath(TEXT("/Script/TP_S.EAbility")),
			static_cast<int32>(EAbility::Confirm), static_cast<int32>(EAbility::Cancel)));

}

void ABasePlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpCount = 0;
	if (bAttackSlam)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, BaseGamePlayTags::Shared_Event_Land, FGameplayEventData());
		bAttackSlam = false;
	}
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// --------------------------------------------------------------------------
	if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
	{
		if (PS->bShouldRestoreData)
		{
			PS->RestoreToComponents(this);
			PS->bShouldRestoreData = false;
			UE_LOG(LogTemp, Warning, TEXT("[Restore] PlayerState에서 데이터 복원 완료"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Restore] 저장된 데이터 없음 → 초기 상태 유지"));
		}
	}
	
}

void ABasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (BaseAbilitySystemComponent)
	{
		BaseAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartupBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			PlayerStartUpData = Cast<UDataAsset_StartupBasePlayer>(LoadedData);
			PlayerStartUpData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);
		}
	}

	ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forget to assigned Startup data to : %s"), *GetName());
}

UAbilitySystemComponent* ABasePlayerCharacter::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
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

void ABasePlayerCharacter::Input_AbilityInputTriggered(const FGameplayTag InputTag)
{
	BaseAbilitySystemComponent->OnAbilityInputTriggered(InputTag);
	if (InputTag == BaseGamePlayTags::InputTag_Attack_Melee_Light)
	{
		bAttackLight = false;
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

UBaseCombatComponent* ABasePlayerCharacter::GetBaseCombatComponent() const
{
	return PlayerCombatComponent;
}
