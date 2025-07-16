// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/InventoryMainWidget.h"

#include "DebugHelper.h"
#include "Character/Player/PreviewPlayerCharacter.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Components/Inventory/QuickSlotComponent.h"
#include "Controllers/BasePlayerController.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"
#include "Items/Inventory/InventorySlotWidget.h"
#include "Items/Inventory/ItemInfoWidget.h"
#include "Items/Weapons/WeaponItemDataAsset.h"

void UInventoryMainWidget::Init(UPlayerInventoryComponent* InInventory)
{
	Inventory = InInventory;

	if (Inventory)
	{
		Inventory->OnInventoryChanged.AddDynamic(this, &UInventoryMainWidget::Refresh);
	}

	if (APawn* P = GetOwningPlayerPawn())
	{
		QuickSlotComponent = P->FindComponentByClass<UQuickSlotComponent>();
		if (QuickSlotComponent)
		{
			QuickSlotComponent->OnQuickSlotChanged.AddDynamic(this, &UInventoryMainWidget::HandleQuickSlotChanged);
		}
	}
	
	Refresh();
}

bool UInventoryMainWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (BtnConsumable)
	{
		BtnConsumable->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnConsumableTab);
	}

	if (BtnMelee)
	{
		BtnMelee->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnMeleeTab);
	}

	if (BtnRanged)
	{
		BtnRanged->OnClicked.AddDynamic(this, &UInventoryMainWidget::OnRangedTab);
	}
	
	return true;
}

void UInventoryMainWidget::OnConsumableTab()
{
	CurrentTab = EInventoryCategory::Consumable;
	Refresh();
}

void UInventoryMainWidget::OnMeleeTab()
{
	CurrentTab = EInventoryCategory::Melee;
	Refresh();
}

void UInventoryMainWidget::OnRangedTab()
{
	CurrentTab = EInventoryCategory::Ranged;
	Refresh();
}

void UInventoryMainWidget::Refresh()
{
	if (!Inventory || !ScrollItems || !SlotClass) return;

	ResetInventoryUI();
	
	const TArray<FItemStack>& Items = Inventory->GetStacks();
	
	for (const FItemStack& Stack : Items)
	{
		if (CurrentTab != EInventoryCategory::None && Stack.ItemData->Category != CurrentTab) continue;

		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotClass);
		SlotWidget->SetupSlot(Stack.ItemData, Stack.Count, Stack.SlotID);

		ApplySlotStates(SlotWidget, Stack);
		BindSlotEvents(SlotWidget);
		ScrollItems->AddChild(SlotWidget);
	}
}

void UInventoryMainWidget::HandleSlotClicked(UInventorySlotWidget* Clicked)
{
	if (!Clicked) return;

	SelectedSlotID = Clicked->SlotID;

	Refresh();
	
	UItemDataAsset* Item = Clicked->GetItem();
	if (!Item) return;

	if (Item->Category == EInventoryCategory::Consumable)
	{
		if (APawn* P = GetOwningPlayerPawn())
		{
			if (auto* QSC = P->FindComponentByClass<UQuickSlotComponent>())
			{
				if (auto* Inv = P->FindComponentByClass<UPlayerInventoryComponent>())
				{
					QSC->RegisterItem(Cast<UConsumableItemDataAsset>(Item), Inv);
				}
			}
		}
	}
	else if (Item->Category == EInventoryCategory::Melee || Item->Category == EInventoryCategory::Ranged)
	{
		FGameplayTag WeaponTag = Item->GetWeaponGameplayTag();
		
		if (APawn* PlayerPawn = GetOwningPlayerPawn())
		{
			if (UBasePlayerCombatComponent* CombatComponent = PlayerPawn->FindComponentByClass<UBasePlayerCombatComponent>())
			{
				UWeaponItemDataAsset* Weapon = Cast<UWeaponItemDataAsset>(Item);
				
				if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer()))
				{
					if (APreviewPlayerCharacter* Preview = PC->PreviewCharacter)
					{
						Preview->AttachWeaponMeshComponent(Weapon->WeaponMesh);
						Preview->PlayEquipMontage(WeaponTag);
					}
				}
				
				if (CombatComponent->CurrentEquippedRangeWeaponTag == WeaponTag || CombatComponent->CurrentEquippedMeleeWeaponTag == WeaponTag)
				{
					UE_LOG(LogTemp, Warning, TEXT("이미 장착된 무기입니다."));
					return;
				}

				CombatComponent->EquipWeaponFromInventory(Weapon->WeaponClass, WeaponTag);
			}
		}
	}
}

void UInventoryMainWidget::HandleQuickSlotChanged(const FQuickSlotData& Data)
{
	Refresh();
}

void UInventoryMainWidget::ShowItemInfo(UItemDataAsset* ItemData)
{
	if (!ItemData || !InfoWidgetHolder || !InfoWidgetClass) return;

	if (CurrentInfoWidget)
	{
		CurrentInfoWidget->RemoveFromParent();
		CurrentInfoWidget = nullptr;
	}

	CurrentInfoWidget = CreateWidget<UItemInfoWidget>(this, InfoWidgetClass);
	CurrentInfoWidget->ItemData = ItemData;

	InfoWidgetHolder->AddChild(CurrentInfoWidget);
}

void UInventoryMainWidget::ApplySlotStates(UInventorySlotWidget* SlotWidget, const FItemStack& Stack)
{
	UItemDataAsset* QuickItem = QuickSlotComponent ? QuickSlotComponent->GetData().ItemData : nullptr;

	UBasePlayerCombatComponent* CombatComp = nullptr;
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		CombatComp = PlayerPawn->FindComponentByClass<UBasePlayerCombatComponent>();
	}
	
	const bool bIsQuick = (QuickItem && Stack.ItemData == QuickItem);
	SlotWidget->SetQuickSlotBg(bIsQuick);

	bool bIsEquippedWeapon = false;
	if (CombatComp && (Stack.ItemData->Category == EInventoryCategory::Melee || Stack.ItemData->Category == EInventoryCategory::Ranged))
	{
		FGameplayTag WeaponTag = Stack.ItemData->GetWeaponGameplayTag();
		bIsEquippedWeapon = (WeaponTag == CombatComp->GetEquippedMeleeTag() || WeaponTag == CombatComp->GetEquippedRangeTag());
	}
	SlotWidget->SetEquippedWeaponBG(bIsEquippedWeapon);
	SlotWidget->SetSelected(Stack.SlotID == SelectedSlotID);
}

void UInventoryMainWidget::ResetInventoryUI()
{
	ScrollItems->ClearChildren();

	if (InfoWidgetHolder)
	{
		InfoWidgetHolder->ClearChildren();
		CurrentInfoWidget = nullptr;
	}
}

void UInventoryMainWidget::BindSlotEvents(UInventorySlotWidget* SlotWidget)
{
	SlotWidget->OnSlotClicked.RemoveDynamic(this, &UInventoryMainWidget::HandleSlotClicked);
	SlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryMainWidget::HandleSlotClicked);

	SlotWidget->OnItemClicked.RemoveDynamic(this, &UInventoryMainWidget::ShowItemInfo);
	SlotWidget->OnItemClicked.AddDynamic(this, &UInventoryMainWidget::ShowItemInfo);
}