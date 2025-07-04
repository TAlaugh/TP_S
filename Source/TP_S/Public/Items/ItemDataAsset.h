// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class TP_S_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** 표기 이름(로컬라이즈 가능) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Basic")
	FText DisplayName;

	/** 인벤토리 아이콘 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Basic")
	TSoftObjectPtr<UTexture2D> Icon;

	/** 아이템 최대 갯수 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	int32 MaxAmount = 1;

	/** 카테고리(탭 전환·필터에 사용) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Basic")
	EInventoryCategory Category = EInventoryCategory::None;

	/** 장착 시 자동 적용할 GE (버프 등) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> ItemEffect;

	/** 공격 시 발동할 GA (콤보, 투사체 발사 등) */
	/** 사용 시 발동할 GA (힐, 버프 등) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UGameplayAbility> PrimaryAbility;

	/** 태그(퀘스트 필터 / 애니 로직 분기용) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS")
	FGameplayTagContainer ItemTags;
};
