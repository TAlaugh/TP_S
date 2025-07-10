// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// Character's Weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	// 현재 장착중인 무기 인스턴스 반환
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABasePlayerWeapon* GetPlayerCurrentEquippedWeaponByTag(FGameplayTag WeaponType) const;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FGameplayTag WeaponType, FName SocketName = FName("hand_rSocket"));

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UnEquipWeapon(FGameplayTag WeaponType);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float Level) const;
	
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

	UPROPERTY()
	FGameplayTag CurrentThrownWeaponTag;

	FName MeleeSocketName = FName("hook_1_back_weaponSocket");
	FName RangeSocketName = FName("hook_2_back_weaponSocket");
	
	// HitDetection
	virtual void OnHitTargetActor(AActor* HitActor);

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleWeaponCollision(bool bUse, EPlayerToggleDamageType ToggleDamageType = EPlayerToggleDamageType::CurrentEquippedWeapon);

protected:
	// 중복방지 배열
	TArray<AActor*> OverlappedActors;
	
	// Character can have variety weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<FGameplayTag, ABasePlayerWeapon*> PlayerWeaponMap;

	UPROPERTY()
	ABasePlayerCharacter* OwnerPlayer;
};
