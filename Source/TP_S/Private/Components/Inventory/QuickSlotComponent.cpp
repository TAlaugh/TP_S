// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/QuickSlotComponent.h"

#include "AbilitySystemComponent.h"
#include "DebugHelper.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Items/Consumables/ConsumableItemDataAsset.h"

void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* P = Cast<APawn>(GetOwner()))
	{
		if (auto* Inventory = P->FindComponentByClass<UPlayerInventoryComponent>())
		{
			Inventory->OnInventoryChanged.AddDynamic(this, &UQuickSlotComponent::HandleInventoryChanged);
		}
	}
}

void UQuickSlotComponent::HandleInventoryChanged()
{
	if (!QuickSlotData.ItemData) return;

	if (APawn* P = Cast<APawn>(GetOwner()))
	{
		if (auto* Inventory = P->FindComponentByClass<UPlayerInventoryComponent>())
		{
			const int32 NewCount = Inventory->CountItem(QuickSlotData.ItemData);

			if (NewCount != QuickSlotData.Count)
			{
				QuickSlotData.Count = NewCount;

				if (NewCount < 0)
				{
					QuickSlotData.ItemData = nullptr;
				}

				OnQuickSlotChanged.Broadcast(QuickSlotData);
			}
		}
	}
}

bool UQuickSlotComponent::RegisterItem(UConsumableItemDataAsset* Item, UPlayerInventoryComponent* Inventory)
{
	if (!Item || !Inventory) return false;
	
	const int32 Existing = Inventory->CountItem(Item);
	
	if (Existing <= 0) return false;

	QuickSlotData.ItemData = Item;
	QuickSlotData.Count = Existing;
	OnQuickSlotChanged.Broadcast(QuickSlotData);
	return true;
}

bool UQuickSlotComponent::UseSlot(UPlayerInventoryComponent* Inventory)
{
	if (!QuickSlotData.ItemData || !Inventory) return false;

	if (Inventory->RemoveOne(QuickSlotData.ItemData))
	{
		OnQuickSlotChanged.Broadcast(QuickSlotData);

		if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
		{
			if (auto* ASC = OwnerChar->FindComponentByClass<UAbilitySystemComponent>())
			{
				// GA가 있는 경우
				if (QuickSlotData.ItemData->PrimaryAbility)
				{
					// Debug::Print(TEXT("ASC->TryActivateAbilityByClass : QuickSlot.Item"));
					ASC->TryActivateAbilityByClass(QuickSlotData.ItemData->PrimaryAbility);
				}
				// GE가 있는 경우
				for (auto GEClass : QuickSlotData.ItemData->ItemEffects)
				{
					if (!GEClass) continue;
					
					// Debug::Print(TEXT("ASC->ApplyGameplayEffectToSelf : QuickSlot.Item"));
					FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, 1.f, ASC->MakeEffectContext());

					if (Spec.IsValid())
					{
						/** 아이템의 수치에 따라 Effect 부여 **/
						if (const auto* ConsumableItem = Cast<UConsumableItemDataAsset>(QuickSlotData.ItemData))
						{
							// HealPercent가 0이 아니면 Tag에 접근해서 특정 수치만큼 힐을 줍니다.
							if (ConsumableItem->HpPercent != 0)
							{
								static const FGameplayTag HpTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Effect.HpPercent"));
								const float MaxHp = ASC->GetNumericAttribute(UBaseAttributeSet::GetMaxHpAttribute());
								const float HealAmt = ConsumableItem->HpPercent * MaxHp;
								Spec.Data->SetSetByCallerMagnitude(HpTag, HealAmt);
							}
							if (ConsumableItem->HpFloat != 0)
							{
								static const FGameplayTag HpTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Effect.HpFloat"));
								Spec.Data->SetSetByCallerMagnitude(HpTag, ConsumableItem->HpFloat);
							}
							if (ConsumableItem->AtkFloat != 0)
							{
								static const FGameplayTag AtkTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Effect.AtkFloat"));
								Spec.Data->SetSetByCallerMagnitude(AtkTag, ConsumableItem->AtkFloat);
							}
						}
						
						ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
					}
				}
			}
		}

		if (QuickSlotData.Count <= 0)
		{
			QuickSlotData.ItemData = nullptr;
			OnQuickSlotChanged.Broadcast(QuickSlotData);
		}
		
		return true;
	}
	return false;
}
