// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/PlayerInventoryComponent.h"

void UPlayerInventoryComponent::GetItemsByCategory(EInventoryCategory Category, TArray<FItemStack>& OutStacks) const
{
	OutStacks.Reset();
	for (const auto& Stacks : Items)
	{
		if (!Stacks.ItemData) continue;

		if (Category == EInventoryCategory::None || Stacks.ItemData->Category == Category)
		{
			OutStacks.Add(Stacks);
		}
	}
}

bool UPlayerInventoryComponent::AddItem(UItemDataAsset* Item, int32 Qty)
{
	if (!Item || Qty <= 0) return false;

	const int32 MaxPerStack = FMath::Max(1, Item->MaxAmount);

	// 기존 스택 찾기
	for (FItemStack& Stack : Items)
	{
		if (Stack.ItemData == Item && Stack.Count < MaxPerStack)
		{
			const int32 Space = MaxPerStack - Stack.Count;
			const int32 Add = FMath::Min(Space, Qty);
			Stack.Count += Add;
			Qty -= Add;
			if (Qty == 0)
			{
				OnInventoryChanged.Broadcast();
				return true;
			}
		}
	}

	// 남은 Qty만큼 새 스택
	while (Qty > 0)
	{
		const int32 Add = FMath::Min(MaxPerStack, Qty);

		FItemStack NewStack;
		NewStack.ItemData = Item;
		NewStack.Count = Add;
		NewStack.SlotID = FGuid::NewGuid();
		
		Items.Add(NewStack);
		Qty -= Add;
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UPlayerInventoryComponent::RemoveAt(int32 Index, int32 Qty)
{
	if (!Items.IsValidIndex(Index) || Qty <= 0) return false;

	FItemStack& Stack = Items[Index];
	if (Qty >= Stack.Count)
	{
		Items.RemoveAt(Index);
	}
	else
	{
		Stack.Count -= Qty;
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UPlayerInventoryComponent::RemoveOne(UItemDataAsset* Item)
{
	if (!Item) return false;

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		FItemStack& Stack = Items[i];
		if (Stack.ItemData == Item)
		{
			if (Stack.Count > 1)
			{
				--Stack.Count;
			}
			else
			{
				Items.RemoveAt(i);
			}

			OnInventoryChanged.Broadcast();
			return true;
		}
	}
	return false;
}

int32 UPlayerInventoryComponent::CountItem(UItemDataAsset* Item) const
{
	if (!Item) return 0;

	int32 Total = 0;
	for (const FItemStack& Stack : Items)
	{
		if (Stack.ItemData == Item)
		{
			Total += Stack.Count;
		}
	}
	
	return Total;
}

const TArray<FItemStack>& UPlayerInventoryComponent::GetAllItemStacks() const
{
	return Items;
}

void UPlayerInventoryComponent::ClearItems()
{
	Items.Empty();
	OnInventoryChanged.Broadcast();
}

int32 UPlayerInventoryComponent::FindStackIndex(UItemDataAsset* Item) const
{
	return Items.IndexOfByPredicate(
		[Item](const FItemStack& Stack){ return Stack.ItemData == Item; }
		);
}
