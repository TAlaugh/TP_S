// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/BaseCombatComponent.h"
#include "GameplayTagContainer.h"
#include "EnemyCombatComponent.generated.h"


UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

class ABaseWeapon;

/**
 * 
 */
UCLASS()
class TP_S_API UEnemyCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()


public:

	//생성된 무기 등록
	UFUNCTION(BlueprintCallable, Category="Enemy")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABaseWeapon* Weapon, bool bRegisterAsEquippedWeapon = false);

	//캐릭터가 휴대하는 무기
	UFUNCTION(BlueprintCallable, Category="Enemy")
	ABaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	//캐릭터가 장착중인 무기의 태그
	UPROPERTY(BlueprintReadWrite, Category="Enemy")
	FGameplayTag CurrentEquippedWeaponTag;

	//캐릭터가 장착한 무기
	UFUNCTION(BlueprintCallable, Category="Enemy")
	ABaseWeapon* GetCharacterCurrentEquippedWeapon() const;

	//무기 콜리전 
	UFUNCTION(BlueprintCallable, Category="Combat")
	void ToggleWeaponCollision(bool bUse, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	//HitDetection
	virtual void OnHitTargetActor(AActor* HitActor) ;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);
	
protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);
	
	//중복방지를 위한 배열
	TArray<AActor*> OverlappedActors;
	
private:
	//캐릭터는 여러 무기를 소유할 수 있다.
	TMap<FGameplayTag, ABaseWeapon*> CharacterCarriedWeaponMap;
	
};
