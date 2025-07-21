// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "Components/Combat/BaseCombatComponent.h"

#include "GameplayTagContainer.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "BasePlayerCombatComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

class ABasePlayerWeapon;
/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:
	
	// 현재 소환된 무기 등록
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon, FGameplayTag WeaponType);

	// 무기소환 해제
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveSpawnedWeapon(FGameplayTag WeaponTag, ABasePlayerWeapon* Weapon, FGameplayTag WeaponType);

	// 무기 자체 태그를 기준으로 현재 보유중인 무기 반환
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCarriedWeaponByWeaponTag(FGameplayTag WeaponTag) const;

	// 현재 장착중인 무기 인스턴스 반환
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	// 현재 던진 무기 인스턴스 반환
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCurrentThrownWeapon() const;

	// 아이템 타입을 기준으로 현재 보유중인 무기 반환(Melee : Item_Equipable_Weapon_Melee, Range : Item_Equipable_Weapon_Range)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCarriedWeaponByTypeTag(FGameplayTag WeaponType) const;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FGameplayTag WeaponType, FName SocketName = FName("hand_rSocket"));

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UnEquipWeapon(FGameplayTag WeaponType);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPlayerCurrentThrownWeaponDamageAtLevel(float Level) const;
	
	// 장착중인 무기의 태그
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentEquippedWeaponTag;
	
	// 근거리
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentEquippedMeleeWeaponTag;

	// 원거리
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentEquippedRangeWeaponTag;

	// 근거리 무기를 던진 위치 저장
	UPROPERTY()
	FVector WeaponThrownLocation;
	// 던진 무기의 태그 저장
	UPROPERTY()
	FGameplayTag CurrentThrownWeaponTag;

	FGameplayTag BaseWeaponTypeMelee = BaseGamePlayTags::Item_Equipable_Weapon_Melee;
	FGameplayTag BaseWeaponTypeRange = BaseGamePlayTags::Item_Equipable_Weapon_Range;
	FName MeleeSocketName = FName("hook_1_back_weaponSocket");
	FName RangeSocketName = FName("hook_2_back_weaponSocket");
	
	// HitDetection
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

	UFUNCTION(BlueprintCallable, Category = "Combat", meta = (DisplayName = "On Hit TargetActor"))
	void BP_OnHitTargetActor(AActor* HitActor);
	
	// 무기 콜리전 변경(타격)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleWeaponCollision(bool bUse, EPlayerToggleDamageType ToggleDamageType = EPlayerToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION()
	void SetMultiHitTimer(bool bUse);

	UFUNCTION()
	bool GetMultiHitTimer() const;

	UPROPERTY()
	FTimerHandle MultiHitTimer;

	UFUNCTION(BlueprintCallable, Category = "Owner")
	UBaseAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UBaseAbilitySystemComponent* GetTargetAbilitySystemComponent(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void MakePlayerDamageFromComponent(AActor* HitActor, float Level, EPlayerToggleDamageType ToggleDamageType = EPlayerToggleDamageType::CurrentEquippedWeapon);

	// 인벤토리에서 무기 호출
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeaponFromInventory(TSubclassOf<ABasePlayerWeapon> WeaponClass, FGameplayTag WeaponTag);

	// WeaponUI
	UFUNCTION()
	UMaterialInterface* GetCurrentWeaponMat();

	UFUNCTION()
	void UpdateWeaponHUD();

	// 저장용도 함수
	FGameplayTag GetEquippedMeleeTag() const;
	ABasePlayerWeapon* GetEquippedMeleeWeaponClass() const;
	FGameplayTag GetEquippedRangeTag() const;
	ABasePlayerWeapon* GetEquippedRangeWeaponClass() const;

	UPROPERTY()
	FGuid CurrentEquippedSlotID;
	
	UFUNCTION()
	void UpdateAnimLayer();

protected:
	// 중복방지 배열
	TArray<AActor*> OverlappedActors;
	
	// Character can have variety weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<FGameplayTag, ABasePlayerWeapon*> PlayerWeaponMap;

	UPROPERTY()
	ABasePlayerCharacter* OwnerPlayer;

	UPROPERTY()
	TSubclassOf<UAnimInstance> CombatLayer;
};
