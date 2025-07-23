// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/Player/BasePlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Effects/GE/GE_DealDamage.h"
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
ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCarriedWeaponByWeaponTag(FGameplayTag WeaponTag) const
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

	return GetPlayerCarriedWeaponByWeaponTag(CurrentEquippedWeaponTag);
}

ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCurrentThrownWeapon() const
{
	if (!CurrentThrownWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetPlayerCarriedWeaponByWeaponTag(CurrentThrownWeaponTag);
}

// 플레이어가 소지하고 있는 무기를 무기타입의 태그로 검색해서 반환
ABasePlayerWeapon* UBasePlayerCombatComponent::GetPlayerCarriedWeaponByTypeTag(FGameplayTag WeaponType) const
{
	if (WeaponType == BaseWeaponTypeMelee && !CurrentEquippedMeleeWeaponTag.IsValid())
	{
		return nullptr;
	}

	if (WeaponType == BaseWeaponTypeRange && !CurrentEquippedRangeWeaponTag.IsValid())
	{
		return nullptr;
	}
	
	return GetPlayerCarriedWeaponByWeaponTag(WeaponType == BaseWeaponTypeMelee ? CurrentEquippedMeleeWeaponTag : CurrentEquippedRangeWeaponTag);
}

// 무기 장착(무기타입{근접,원거리} | 소켓)
void UBasePlayerCombatComponent::EquipWeapon(FGameplayTag WeaponType, FName SocketName)
{
	if (!GetPlayerCarriedWeaponByTypeTag(WeaponType))
	{
		return;
	}

	// 현재 손에 들고있는 무기랑 장착하려는 무기랑 다른 경우 WeaponType으로 분기해서 미리 장착을 해제하고 다시 Equip
	if (GetPlayerCurrentEquippedWeapon() != nullptr && GetPlayerCurrentEquippedWeapon() != GetPlayerCarriedWeaponByTypeTag(WeaponType))
	{
		UnEquipWeapon(WeaponType == BaseWeaponTypeMelee ? BaseWeaponTypeRange : BaseWeaponTypeMelee);
	}
	
	GetPlayerCarriedWeaponByTypeTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	if (WeaponType == BaseWeaponTypeMelee)
	{
		CurrentEquippedWeaponTag = CurrentEquippedMeleeWeaponTag;
		GetPlayerCarriedWeaponByTypeTag(WeaponType)->DissolveFXWeapon(false);
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
	if (!GetPlayerCarriedWeaponByTypeTag(WeaponType))
	{
		return;
	}
	
	FName SocketName;
	if (WeaponType == BaseWeaponTypeMelee)
	{
		SocketName = MeleeSocketName;
		GetPlayerCarriedWeaponByTypeTag(WeaponType)->DissolveFXWeapon(true);
	} else
	{
		GetPlayerCurrentEquippedWeapon()->GetSkeletalMeshComponent()->SetVisibility(false);
		SocketName = RangeSocketName;
	}
	
	GetPlayerCarriedWeaponByTypeTag(WeaponType)->AttachToComponent(
		GetOwningPawn()->FindComponentByClass<USkeletalMeshComponent>(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName);
	
	CurrentEquippedWeaponTag = FGameplayTag();
	OverlappedActors.Empty();
}

// TODO::현재 장착중인 무기의 데미지를 가져옴
float UBasePlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const
{
	if (GetPlayerCurrentEquippedWeapon())
	{
		return GetPlayerCurrentEquippedWeapon()->ItemDataAsset->BaseDamage; //GetValueAtLevel(Level);
	}
	return Level;

}

float UBasePlayerCombatComponent::GetPlayerCurrentThrownWeaponDamageAtLevel(float Level) const
{
	if (GetPlayerCurrentThrownWeapon())
	{
		return GetPlayerCurrentThrownWeapon()->ItemDataAsset->BaseDamage; //GetValueAtLevel(Level);
	}
	return Level;
}

// 무기의 콜리전에 닿았을 때(피격 시)
void UBasePlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
	{
		if (OverlappedActors.Contains(HitActor))
		{
			return;
		}

		OverlappedActors.AddUnique(HitActor);

		if (GetMultiHitTimer())
		{
			// 컴포넌트 기반으로 데미지 처리
			MakePlayerDamageFromComponent(HitActor, 1);
		}
		else
		{
			// GA 내에서 WaitGameplayEvent로 데미지 처리
			FGameplayEventData Data;
			Data.Instigator = GetOwningPawn();
			Data.Target = HitActor;

			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BaseGamePlayTags::Shared_Event_Hit, Data);
		}
	}
}

void UBasePlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}

void UBasePlayerCombatComponent::BP_OnHitTargetActor(AActor* HitActor)
{
	OnHitTargetActor(HitActor);
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
				// 무기 콜리전이 켜졌을 때 무기투척이면 OnHitTargetActor에서 사용하는 OverlappedActor를 주기적으로 초기화시켜준다.
				if (CurrentThrownWeaponTag.IsValid())
				{
					SetMultiHitTimer(true);
				}
			}
			else
			{
				Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				OverlappedActors.Empty();
				SetMultiHitTimer(false);
			}
		}
	}
	
}

void UBasePlayerCombatComponent::SetMultiHitTimer(bool bUse)
{
	if (bUse && !MultiHitTimer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			MultiHitTimer,
			FTimerDelegate::CreateLambda([this]()
			{
				OverlappedActors.Empty();
			}),
			.25f,
			true);
	}
	else
	{
		if (MultiHitTimer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(MultiHitTimer);
		}
	}
}

bool UBasePlayerCombatComponent::GetMultiHitTimer() const
{
	return MultiHitTimer.IsValid();
}

UBaseAbilitySystemComponent* UBasePlayerCombatComponent::GetOwnerAbilitySystemComponent() const
{
	if (OwnerPlayer)
	{
		return Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerPlayer));
	}
	return nullptr;
}

UBaseAbilitySystemComponent* UBasePlayerCombatComponent::GetTargetAbilitySystemComponent(AActor* TargetActor) const
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		return Cast<UBaseAbilitySystemComponent>(ASC);
	}
	return nullptr;
}

void UBasePlayerCombatComponent::MakePlayerDamageFromComponent(AActor* HitActor, float Level, EPlayerToggleDamageType ToggleDamageType)
{
	TSubclassOf<UGameplayEffect> Effect = UGE_DealDamage::StaticClass();
	float BaseDamage;
	if (ToggleDamageType == EPlayerToggleDamageType::CurrentEquippedWeapon)
	{
		BaseDamage = GetPlayerCurrentEquippedWeaponDamageAtLevel(Level);
	}
	else
	{
		BaseDamage = GetPlayerCurrentEquippedWeaponDamageAtLevel(Level);
	}
		
	//NativeApplyEffectSpecHandleToTarget()
	UAbilitySystemComponent* TargetASC = GetTargetAbilitySystemComponent(HitActor);
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeApplyEffectSpecHandleToTarget: ASC is nullptr! TargetActor: %s"), *GetNameSafe(HitActor));
		return;
	}
	UBaseAbilitySystemComponent* OwnerASC = GetOwnerAbilitySystemComponent();
	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeApplyEffectSpecHandleToTarget: ASC is nullptr! Owner: %s"), *GetNameSafe(OwnerPlayer));
		return;
	}
		
	FGameplayEffectContextHandle ContextHandle;
	ContextHandle.AddSourceObject(OwnerPlayer);
	ContextHandle.AddInstigator(OwnerPlayer, OwnerPlayer);

	FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Effect, 1, ContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(BaseGamePlayTags::Shared_SetByCaller_BaseDamage, BaseDamage);
		
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("NativeApplyEffectSpecHandleToTarget: SpecHandle is invalid!"));
		return;
	}

	OwnerASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, BaseGamePlayTags::Shared_Event_HitReact, FGameplayEventData());
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

