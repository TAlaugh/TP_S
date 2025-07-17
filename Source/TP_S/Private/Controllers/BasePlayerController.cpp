// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BasePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Character/Player/PreviewPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"
#include "Items/Inventory/InventoryMainWidget.h"
#include "Items/Inventory/QuickSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/HUDWidget.h"

ABasePlayerController::ABasePlayerController()
{
	bShowMouseCursor = false;
	bShouldPerformFullTickWhenPaused = true;
	TeamId = FGenericTeamId(0);
}

FGenericTeamId ABasePlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	
	UAbilitySystemComponent* ASC = PlayerCharacter->GetBaseAbilitySystemComponent();
	UBaseAttributeSet* AttributeSet = PlayerCharacter->GetBaseAttributeSet();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (IMC_Default)
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
	
	if (APawn* P = GetPawn())
	{
		InventoryComponent = P->FindComponentByClass<UPlayerInventoryComponent>();
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryMainWidget>(this, InventoryWidgetClass);
		if (InventoryWidget && InventoryComponent)
		{
			InventoryWidget->Init(InventoryComponent);
		}
	}
	
	if (PlayerHUDClass)
	{
		UHUDWidget* HUD = CreateWidget<UHUDWidget>(GetWorld(), PlayerHUDClass);
		if (HUD)
		{
			PlayerHUDWidget = HUD;
			HUD->AddToViewport();
			HUD->BindToAttribute(ASC, AttributeSet);

			if (auto* QSC = GetPawn()->FindComponentByClass<UQuickSlotComponent>())
			{
				HUD->BindToQuickSlot(QSC);
			}
		}
	}

	// 시작할 때 무기 지급
	if (InventoryComponent)
	{
		auto LoadItem = [](const TCHAR* Path)
		{
			return Cast<UItemDataAsset>(StaticLoadObject(UItemDataAsset::StaticClass(), nullptr, Path));
		};
		
		if (auto* WeaponMelee = LoadItem(TEXT("/Game/Common/_BP/Items/DA_Item_Weapon_Xiji.DA_Item_Weapon_Xiji")))
		{
			InventoryComponent->AddItem(WeaponMelee);
		}

		if (auto* WeaponRanged = LoadItem(TEXT("/Game/Common/_BP/Items/DA_Item_Weapon_Yinlang.DA_Item_Weapon_Yinlang")))
		{
			InventoryComponent->AddItem(WeaponRanged);
		}
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_ToggleInventory)
		{
			EIC->BindAction(IA_ToggleInventory, ETriggerEvent::Started, this, &ABasePlayerController::OnToggleInventory);
		}
		if (IA_UseQuickSlot)
		{
			EIC->BindAction(IA_UseQuickSlot, ETriggerEvent::Started, this, &ABasePlayerController::OnUseQuickSlot);
		}
	}
}

void ABasePlayerController::OnToggleInventory(const FInputActionValue& Value)
{
	bInventoryOpen ? HideInventory() : ShowInventory();
}

void ABasePlayerController::ShowInventory()
{
	if (!InventoryWidget) return;

	if (!InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
	}

	if (PlayerHUDWidget && PlayerHUDWidget->IsInViewport())
	{
		PlayerHUDWidget->RemoveFromParent();
	}
	
	InventoryWidget->Refresh();
	
	FInputModeGameAndUI Mode;
	Mode.SetWidgetToFocus(InventoryWidget->TakeWidget());
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);

	bShowMouseCursor = true;
	bInventoryOpen = true;

	// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	SpawnPreviewCharacter();

	if (PreviewCharacter)
	{
		SetViewTargetWithBlend(PreviewCharacter, 0.0f);
		PreviewCharacter->PlayEnterMontage();
	}
}

void ABasePlayerController::HideInventory()
{
	if (!InventoryWidget) return;

	InventoryWidget->RemoveFromParent();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	bInventoryOpen = false;

	// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	// PreviewCharacter->CustomTimeDilation = 1.f;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	if (PreviewCharacter)
	{
		PreviewCharacter->Destroy();
		PreviewCharacter = nullptr;
	}

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void ABasePlayerController::SpawnPreviewCharacter()
{
	if (!PreviewCharacterClass) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// FVector PreviewSpawnLocation(0.f, 0.f, 1000.f);
	// FRotator PreviewSpawnRotation = FRotator::ZeroRotator;
	
	FVector BaseLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator PreviewSpawnRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

	FHitResult HitResult;
	FVector TraceStart = BaseLocation + FVector(0, 0, 50);
	FVector TraceEnd = BaseLocation - FVector(0, 0, 1000);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPawn());

	FVector PreviewSpawnLocation = BaseLocation;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		ABasePlayerCharacter* PlayerChar = Cast<ABasePlayerCharacter>(GetPawn());
		PreviewSpawnLocation.Z = HitResult.ImpactPoint.Z + PlayerChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	
	PreviewCharacter = GetWorld()->SpawnActor<APreviewPlayerCharacter>(PreviewCharacterClass, PreviewSpawnLocation, PreviewSpawnRotation, Params);

	if (ABasePlayerCharacter* PlayerChar = Cast<ABasePlayerCharacter>(GetPawn()))
	{
		PreviewCharacter->CopyMeshFromPlayer(PlayerChar);
	}
}

void ABasePlayerController::OnUseQuickSlot(const FInputActionValue& Value)
{
	if (auto* QSC = GetPawn()->FindComponentByClass<UQuickSlotComponent>())
	{
		if (auto* Inventory = GetPawn()->FindComponentByClass<UPlayerInventoryComponent>())
		{
			QSC->UseSlot(Inventory);
		}
	}
}

void ABasePlayerController::GiveItems()
{
	if (APawn* MyPawn = GetPawn())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, (TEXT("%s"), *MyPawn->GetActorNameOrLabel()));
		if (UPlayerInventoryComponent* Inv = MyPawn->FindComponentByClass<UPlayerInventoryComponent>())
		{
			// Debug::Print(TEXT("Call GiveItems"));
			
			auto LoadItem = [](const TCHAR* Path)
			{
				return Cast<UItemDataAsset>(StaticLoadObject(UItemDataAsset::StaticClass(), nullptr, Path));
			};

			if (auto* Potion = LoadItem(TEXT("/Game/Common/_BP/Items/DA_Item_Consumable_ActionPotion.DA_Item_Consumable_ActionPotion")))
			{
				bool bAdded = Inv->AddItem(Potion, 3);
				// Debug::Print(FString::Printf(TEXT("Potion add %s"), bAdded ? TEXT("OK") : TEXT("FAIL")));
			}

			if (auto* Potion2 = LoadItem(TEXT("/Game/Common/_BP/Items/DA_Item_Consumable_HpPotion.DA_Item_Consumable_HpPotion")))
			{
				Inv->AddItem(Potion2, 5);
			}
		}
	}
}
