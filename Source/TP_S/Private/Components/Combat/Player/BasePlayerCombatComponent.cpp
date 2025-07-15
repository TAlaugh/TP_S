// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AnimInstances/Player/BasePlayerLinkedAnimLayer.h"
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
		if (OwnerPlayer->GetMesh()->GetAnimInstance() && Weapon->PlayerWeaponData.WeaponAnimLayerLink != nullptr)
		{
			OwnerPlayer->GetMesh()->GetAnimInstance()->LinkAnimClassLayers(Weapon->PlayerWeaponData.WeaponAnimLayerLink);
			CombatLayer = Weapon->PlayerWeaponData.WeaponAnimLayerLink;
		}
	}
	
	if (ULocalPlayer* LocalPlayer = GetOwningPawn()->GetController<ABasePlayerController>()->GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			Subsystem->AddMappingContext(Weapon->PlayerWeaponData.WeaponInputMappingContext, 1);
			UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseASCFromActor(GetOwningPawn());
			ASC->GrantPlayerWeaponAbilities(Weapon->PlayerWeaponData.WeaponAbilities, 1, Weapon);
		}
	}
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
		if (OwnerPlayer->GetMesh()->GetAnimInstance() && Weapon->PlayerWeaponData.WeaponAnimLayerLink != nullptr)
		{
			OwnerPlayer->GetMesh()->GetAnimInstance()->UnlinkAnimClassLayers(Weapon->PlayerWeaponData.WeaponAnimLayerLink);
			CombatLayer = nullptr;
		}
	}
	if (ULocalPlayer* LocalPlayer = GetOwningPawn()->GetController<ABasePlayerController>()->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(Weapon->PlayerWeaponData.WeaponInputMappingContext, 1);
			UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseASCFromActor(GetOwningPawn());
			//ASC->RemoveGrantedPlayerWeaponAbilities(Weapon->GetGrantedAbilitySpecHandles());
		}
	}
	
	Weapon->Destroy();
}

// 플레이어가 소지하고 있는 무기를 태그로 검색
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

// 현재 손에 들고있는 무기를 반환
ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetPlayerCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

// 플레이어가 소지하고 있는 무기를 무기타입의 태그로 검색해서 반환
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

// 무기 장착(무기타입{근접,원거리} | 소켓)
void UBasePlayerCombatComponent::EquipWeapon(FGameplayTag WeaponType, FName SocketName)
{
	if (!GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		return;
	}

	// 현재 손에 들고있는 무기랑 장착하려는 무기랑 다른 경우 WeaponType으로 분기해서 미리 장착을 해제하고 다시 Equip
	if (GetPlayerCurrentEquippedWeapon() != nullptr && GetPlayerCurrentEquippedWeapon() != GetPlayerCurrentEquippedWeaponByTag(WeaponType))
	{
		UnEquipWeapon(WeaponType == BaseWeaponTypeMelee ? BaseWeaponTypeRange : BaseWeaponTypeMelee);
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
	//UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Equipped Weapon Tag: %s"), *CurrentEquippedWeaponTag.ToString());
	
	UpdateWeaponHUD();
	
}

// 무기 장착해제(무기타입{근접,원거리})
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

// TODO::현재 장착중인 무기의 데미지를 가져옴
float UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const
{
	//return GetPlayerCurrentEquippedWeapon()->data.WeaponBaseDamage.GetValueAtLevel(Level);
	return Level;
}

// 무기의 콜리전에 닿았을 때(피격 시)
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

// 무기의 콜리전 변경(근접무기 전용)
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

// 인벤토리에서 무기 장착(태그, Class 기반)
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

// 현재 손에 들고 있는 무기의 Material반환
UMaterialInterface* UBasePlayerCombatComponent::GetCurrentWeaponMat()
{
	if (PlayerWeaponMap.Contains(CurrentEquippedWeaponTag))
	{
		ABasePlayerWeapon* Weapon = PlayerWeaponMap[CurrentEquippedWeaponTag];
		return Weapon ? Weapon->ItemDataAsset->EquipWeaponMaterial : nullptr;
	}
	return nullptr;
}

// 무기UI 업데이트
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

// 근접무기의 태그 반환
FGameplayTag UBasePlayerCombatComponent::GetEquippedMeleeTag() const
{
	return CurrentEquippedMeleeWeaponTag;
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetEquippedMeleeWeaponClass() const
{
	if (PlayerWeaponMap.Contains(CurrentEquippedMeleeWeaponTag))
	{
		ABasePlayerWeapon* Weapon = PlayerWeaponMap[CurrentEquippedMeleeWeaponTag];
		return Weapon ? Weapon : nullptr;
	}
	return nullptr;
}

// 원거리무기의 태그 반환
FGameplayTag UBasePlayerCombatComponent::GetEquippedRangeTag() const
{
	return CurrentEquippedRangeWeaponTag;
}

void UBasePlayerCombatComponent::UpdateAnimLayer()
{
	if (CombatLayer != nullptr)
	{
		OwnerPlayer->GetMesh()->GetAnimInstance()->LinkAnimClassLayers(CombatLayer);
	}
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetEquippedRangeWeaponClass() const
{
	if (PlayerWeaponMap.Contains(CurrentEquippedRangeWeaponTag))
	{
		ABasePlayerWeapon* Weapon = PlayerWeaponMap[CurrentEquippedRangeWeaponTag];
		return Weapon ? Weapon : nullptr;
	}
	return nullptr;
}

