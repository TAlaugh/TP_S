// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FItemInfoRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText Label;

	UPROPERTY(BlueprintReadOnly)
	FText Value;
};

/**
 * 
 */
UCLASS()
class TP_S_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 표기 이름(로컬라이즈 가능)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FText DisplayName;

	// 인벤토리 아이콘
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TSoftObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TSoftObjectPtr<UMaterialInterface> IconMaterial;

	// 아이템 최대 갯수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	int32 MaxAmount = 1;

	// 카테고리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	EInventoryCategory Category = EInventoryCategory::None;

	// 장착 / 사용 시 자동 적용할 GE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> ItemEffects;
	
	// 장착 / 사용 시 발동할 GA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UGameplayAbility> PrimaryAbility;

	// 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	FGameplayTag ItemTags;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FGameplayTag GetWeaponGameplayTag() const { return ItemTags; }
	
	// UI 표시
	virtual void GatherDisplayRows(TArray<FItemInfoRow>& OutRows) const;
	
};
