// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"


#include "AbilitySystemBlueprintLibrary.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Player/PlayerAttributeSet.h"
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

ABasePlayerCharacter::ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 65.f);
	//CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//PlayerAttributeSet = Cast<UPlayerAttributeSet>(BaseAttributeSet);
	//ensure(PlayerAttributeSet);
	
	UPlayerMovementComponent* PlayerMoveComp = CastChecked<UPlayerMovementComponent>(GetCharacterMovement());
	PlayerMoveComp->GravityScale = 1.5f;
	PlayerMoveComp->MaxAcceleration = 2400.0f;
	PlayerMoveComp->BrakingFrictionFactor = 1.0f;
	PlayerMoveComp->BrakingFriction = 6.0f;
	PlayerMoveComp->GroundFriction = 8.0f;
	PlayerMoveComp->bUseControllerDesiredRotation = false;
	PlayerMoveComp->bOrientRotationToMovement = true;
	PlayerMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	PlayerMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanJump = true;
	PlayerMoveComp->GetNavAgentPropertiesRef().bCanWalk = true;
	PlayerMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	PlayerMoveComp->MaxWalkSpeed = 400.f;
	PlayerMoveComp->BrakingDecelerationWalking = 2000.f;

	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
	PlayerQuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UBasePlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	GetMesh()->SetHiddenInSceneCapture(true);

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

	if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
	{
		if (PS->bShouldRestoreData)
		{
			if (!PS->StoredCombatInfo.MeleeWeaponTag.IsValid() || !PS->StoredCombatInfo.RangeWeaponTag.IsValid())
			{
				// 아직 데이터가 복제되지 않음 → 0.1초 뒤 재시도
				GetWorldTimerManager().SetTimerForNextTick([=, this]()
				{
					TryRestoreAfterReplication(PS);
					UE_LOG(LogTemp, Warning, TEXT("아직 데이터가 복제되지 않음 → 재시도"));
				});
			}
			else
			{
				// 데이터 바로 복원 가능
				PS->RestoreToComponents(this);
				PS->bShouldRestoreData = false;
				UE_LOG(LogTemp, Warning, TEXT("PlayerState 복원 수행 (바로 가능)"));
			}
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

void ABasePlayerCharacter::OnDead()
{
	Super::OnDead();
	GetMesh()->bPauseAnims = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABasePlayerCharacter::TryRestoreAfterReplication(ABasePlayerState* PS)
{
	if (PS)
	{
		if (PS->StoredCombatInfo.MeleeWeaponTag.IsValid() || PS->StoredCombatInfo.RangeWeaponTag.IsValid() || PS->StoredInventory.Num() > 0)
		{
			PS->RestoreToComponents(this);
			UE_LOG(LogTemp, Warning, TEXT("[TryRestoreAfterReplication] 복원 완료 - PlayerCharacter"));
		}
		else
		{
			FTimerHandle TimerHandle;
			UE_LOG(LogTemp, Warning, TEXT("[TryRestoreAfterReplication] 복원 실패 - PlayerCharacter"));
			GetWorldTimerManager().SetTimer( TimerHandle, FTimerDelegate::CreateLambda([this, PS]()
				{
					TryRestoreAfterReplication(PS);
				}),0.1f,false);
		}
	}
}
