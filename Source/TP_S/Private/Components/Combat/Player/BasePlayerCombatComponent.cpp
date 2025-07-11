// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "AbilitySystem/Player/PlayerAbilitySystemComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Controllers/BasePlayerController.h"
#include "Items/Weapons/BasePlayerWeapon.h"
#include "Items/Weapons/WeaponItemDataAsset.h"
#include "Widget/HUDWidget.h"
#include "Widget/WeaponHUDWidget.h"

void UBasePlayerCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon, FGameplayTag WeaponType)
{
	checkf(!PlayerWeaponMap.Contains(WeaponTag), TEXT("Already Equipped"), *WeaponTag.ToString());

	check(Weapon);

	PlayerWeaponMap.Emplace(WeaponTag, Weapon);
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (WeaponType == BaseWeaponTypeMelee){
		CurrentEquippedMeleeWeaponTag = WeaponTag;
	} else
	{
		CurrentEquippedRangeWeaponTag = WeaponTag;
		Weapon->GetSkeletalMeshComponent()->SetVisibility(false);
	}
	Debug::Print(WeaponTag.ToString());
}

void UBasePlayerCombatComponent::RemoveSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon, FGameplayTag WeaponType)
{
	checkf(PlayerWeaponMap.Contains(WeaponTag), TEXT("Alreay Removed"), *WeaponTag.ToString());

	check(Weapon);
	
	PlayerWeaponMap.Remove(WeaponTag);
	Weapon->OnWeaponHitTarget.Unbind();
	Weapon->OnWeaponPulledFromTarget.Unbind();

	if (WeaponType == BaseWeaponTypeMelee)
	{
		CurrentEquippedMeleeWeaponTag = FGameplayTag();
	}
	else
	{
		CurrentEquippedRangeWeaponTag = FGameplayTag();
	}
	Weapon->Destroy();
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (PlayerWeaponMap.Contains(WeaponTag))
	{
		if (ABasePlayerWeapon* const* FoundWeapon = PlayerWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetPlayerCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponByTag(FGameplayTag WeaponType) const
{
	if (WeaponType == BaseWeaponTypeMelee && !CurrentEquippedMeleeWeaponTag.IsValid())
	{
		return nullptr;
	}

	if (WeaponType == BaseWeaponTypeRange && !CurrentEquippedRangeWeaponTag.IsValid())
	{
		return nullptr;
	}
	
	return GetPlayerCarriedWeaponByTag(WeaponType == BaseWeaponTypeMelee ? CurrentEquippedMeleeWeaponTag : CurrentEquippedRangeWeaponTag);
}

void UBasePlayerCombatComponent::EquipWeapon(FGameplayTag WeaponType, FName SocketName)
{
	if (!GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		return;
	}
	
	GetPlayerCurrentEquippedWeaponByTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	if (WeaponType == BaseWeaponTypeMelee)
	{
		CurrentEquippedWeaponTag = CurrentEquippedMeleeWeaponTag;
	}
	else
	{
		CurrentEquippedWeaponTag = CurrentEquippedRangeWeaponTag;
		GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->SetVisibility(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Equipped Weapon Tag: %s"), *CurrentEquippedWeaponTag.ToString());

	UpdateWeaponHUD();
}

void UBasePlayerCombatComponent::UnEquipWeapon(FGameplayTag WeaponType)
{
	if (!GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		return;
	}
	
	FName SocketName;
	if (WeaponType == BaseWeaponTypeMelee)
	{
		SocketName = MeleeSocketName;
	} else
	{
		GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->SetVisibility(false);
		SocketName = RangeSocketName;
	}
	
	GetPlayerCurrentEquippedWeaponByTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	CurrentEquippedWeaponTag = FGameplayTag();
}

float UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const
{
	//return GetPlayerCurrentEquippedWeapon()->data.WeaponBaseDamage.GetValueAtLevel(Level);
	return Level;
}

void UBasePlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);
	
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_Hit, Data);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_HitReact, FGameplayEventData());
}

void UBasePlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}

void UBasePlayerCombatComponent::ToggleWeaponCollision(bool bUse, EPlayerToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EPlayerToggleDamageType::CurrentEquippedWeapon)
	{
		ABasePlayerWeapon* Weapon = GetPlayerCurrentEquippedWeapon();

		if (Weapon)
		{
			if (bUse)
			{
				Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
			else
			{
				Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				OverlappedActors.Empty();
			}
		}
	}
	
}

void UBasePlayerCombatComponent::EquipWeaponFromInventory(TSubclassOf<ABasePlayerWeapon> WeaponClass, FGameplayTag WeaponTag)
{
	if (!OwnerPlayer)
	{
		OwnerPlayer = Cast<ABasePlayerCharacter>(GetOwner());
		if (!OwnerPlayer) return;
	}
	FGameplayEventData Data;
	Data.OptionalObject = WeaponClass;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerPlayer, BaseGamePlayTags::Player_Event_Equip, Data);

	return;
	UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseASCFromActor(OwnerPlayer);
	if (!ASC) return;
	
	FGameplayTag MeleeWeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipable.Weapon.Melee"));
	FGameplayTag RangeWeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipable.Weapon.Range"));

	bool bIsMeleeWeapon = WeaponTag.MatchesTag(MeleeWeaponTag);
	bool bIsRangeWeapon = WeaponTag.MatchesTag(RangeWeaponTag);
	
	if (!bIsMeleeWeapon && !bIsRangeWeapon)
	{
		//Debug::Print(TEXT("Unknown Weapon Type"));
		return;
	}

	FGameplayTag OldTag = bIsMeleeWeapon ? CurrentEquippedMeleeWeaponTag : CurrentEquippedRangeWeaponTag;
	if (ASC->HasMatchingGameplayTag(OldTag))
	{
		ASC->RemoveLooseGameplayTag(OldTag);
		UnEquipWeapon(bIsMeleeWeapon ? MeleeWeaponTag : RangeWeaponTag);
	}

	ASC->AddLooseGameplayTag(WeaponTag);
	if (bIsMeleeWeapon)
	{
		CurrentEquippedMeleeWeaponTag = WeaponTag;
		EquipWeapon(MeleeWeaponTag, FName("hook_1_back_weaponSocket"));
	}
	else
	{
		CurrentEquippedRangeWeaponTag = WeaponTag;
		EquipWeapon(RangeWeaponTag, FName("hook_2_back_weaponSocket"));
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Selected Weapon Tag: %s"), *WeaponTag.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Current Melee Tag: %s"), *CurrentEquippedMeleeWeaponTag.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Current Range Tag: %s"), *CurrentEquippedRangeWeaponTag.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("ASC has Melee Tag? %s"), ASC->HasMatchingGameplayTag(CurrentEquippedMeleeWeaponTag) ? TEXT("Yes") : TEXT("No"));
	//UE_LOG(LogTemp, Warning, TEXT("ASC has Range Tag? %s"), ASC->HasMatchingGameplayTag(CurrentEquippedRangeWeaponTag) ? TEXT("Yes") : TEXT("No"));
}

UMaterialInterface* UBasePlayerCombatComponent::GetCurrentWeaponMat()
{
	if (PlayerWeaponMap.Contains(CurrentEquippedWeaponTag))
	{
		ABasePlayerWeapon* Weapon = PlayerWeaponMap[CurrentEquippedWeaponTag];
		return Weapon ? Weapon->ItemDataAsset->EquipWeaponMaterial : nullptr;
	}
	return nullptr;
}

void UBasePlayerCombatComponent::UpdateWeaponHUD()
{
	ABasePlayerCharacter* OwnerChar = Cast<ABasePlayerCharacter>(GetOwner());
	if (!OwnerChar) return;

	ABasePlayerController* PC = Cast<ABasePlayerController>(OwnerChar->GetController());
	if (!PC) return;

	UHUDWidget* PlayerHUDWidget = PC->GetPlayerHUDWidget();
	if (!PlayerHUDWidget) return;

	UWeaponHUDWidget* WeaponHUDWidget = PlayerHUDWidget->GetWeaponHUDWidget();

	if (WeaponHUDWidget)
	{
		UMaterialInterface* Mat = GetCurrentWeaponMat();
		WeaponHUDWidget->UpdateWeaponDisplay(Mat);
	}
}

// Debug용 입니다. 지워도 무방
void UBasePlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Current Melee Tag: %s"), *CurrentEquippedMeleeWeaponTag.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Current Range Tag: %s"), *CurrentEquippedRangeWeaponTag.ToString());
}

