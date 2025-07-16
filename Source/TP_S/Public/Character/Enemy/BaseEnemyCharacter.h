// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_S/Public/Character/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/UI/BaseUIComponent.h"
#include "DataAssets/DataAsset_StartupBase.h"
#include "Interfaces/BaseCombatInterface.h"
#include "Interfaces/BaseUIInterface.h"
#include "BaseEnemyCharacter.generated.h"


class UEnemyCombatComponent;
class UBaseCombatComponent;
class UMotionWarpingComponent;
class UWidgetComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class TP_S_API ABaseEnemyCharacter : public ABaseCharacter, public IBaseUIInterface 
{
	GENERATED_BODY()


	public:
	ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual UBaseCombatComponent* GetBaseCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	
	
	
	protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

#if WITH_EDITOR
	//게임내에서 캐릭터 본네임으로 바꾸는거
	//uobject에서 인터페이스에서 시작해서
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//uobject 인터페이스에서 끝
#endif
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StartupData")
	TSoftObjectPtr<UDataAsset_StartupBase> StartupData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FName RightHandCollisionBoxAttachBoneName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	UBaseCombatComponent* BaseCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UEnemyUIComponent* EnemyUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual UBaseCombatComponent* GetBaseCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const; //override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;// override;
	

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const {return LeftHandCollisionBox;}
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const {return RightHandCollisionBox;}
};
