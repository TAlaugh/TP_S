#pragma once

#include "GameplayTagContainer.h"
#include "PlayerEnumType.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility.h"
#include "PlayerStructType.generated.h"

class UInputMappingContext;
class UBasePlayerLinkedAnimLayer;
class UPlayerGameplayAbility;

USTRUCT(BlueprintType)
struct FPlayerAbilitiySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Catgories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerGameplayAbility> AbilityToGrant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAbility InputId = EAbility::None;

	bool IsValid() const
	{
		return InputTag.IsValid() && AbilityToGrant;
	};
	
};
USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

	// 무기 애님레이어링크(원거리 사격 시 이동 레이어)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBasePlayerLinkedAnimLayer> WeaponAnimLayerLink;
	// 무기 입력 바운딩용 매핑컨텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;
	// 무기 능력(기본공격, 강공)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerAbilitiySet> WeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, UAnimMontage*> WeaponAbilityMontages;
};