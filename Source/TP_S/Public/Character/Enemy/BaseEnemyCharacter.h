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
#include "NiagaraSystem.h"        // UNiagaraSystem 타입 사용하려면 필요
#include "NiagaraFunctionLibrary.h" // Niagara 스폰할 때 필요
#include "Sound/SoundBase.h"
	
#include "BaseEnemyCharacter.generated.h"


class UEnemyCombatComponent;
class UBaseCombatComponent;
class UMotionWarpingComponent;
class UWidgetComponent;
class UBoxComponent;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, ABaseEnemyCharacter*, Enemy);

UCLASS()
class TP_S_API ABaseEnemyCharacter : public ABaseCharacter, public IBaseUIInterface 
{
	GENERATED_BODY()


	public:
	ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual UBaseCombatComponent* GetBaseCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	virtual void Die();
	virtual void OnHealthChanged(float NewHealth, float MaxHealth);
	// ✅ 보스인지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BossPhase")
	bool bIsBoss = false;

	// ✅ 2페이즈 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BossPhase")
	bool bIsPhaseTwo = false;
	
	UPROPERTY(BlueprintAssignable, Category="Enemy")
	FOnEnemyDied OnEnemyDied;

	FTimerHandle SmoothDarkTimer;
	float CurrentBlendWeight = 0.0f;  // 시작값
	float TargetBlendWeight = 1.0f; // 종료값
	
	
	
	protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	void PlayLightningEffect();
	void UpdatePhaseTwoDarkness();
	// ✅ 2페이즈 진입 시 호출될 함수
	virtual void EnterPhaseTwo();

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

	UPROPERTY(EditAnywhere, Category="PhaseTwo")
	APostProcessVolume* PhaseTwoPostProcessVolume;

	UPROPERTY(EditAnywhere, Category="PhaseTwo|Effects")
	UNiagaraSystem* RainEffect;

	UPROPERTY(EditAnywhere, Category="PhaseTwo|Effects")
	USoundBase* RainSound;

	UPROPERTY(EditAnywhere, Category="PhaseTwo|Effects")
	USoundBase* ThunderSound;

	UPROPERTY(EditAnywhere, Category="PhaseTwo|Effects")
	UNiagaraSystem* LightningEffect;
	
	FTimerHandle LightningTimerHandle;

	virtual void HandleDeath();

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void InitEnemyStartUpData();
	

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const {return LeftHandCollisionBox;}
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const {return RightHandCollisionBox;}
};
