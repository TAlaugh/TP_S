// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/Player/PlayerStructType.h"
#include "Items/Weapons/BaseWeapon.h"
#include "BasePlayerWeapon.generated.h"

class UWeaponItemDataAsset;
class UPlayerWeaponDataAsset;
/**
 * 
 */
UCLASS()
class TP_S_API ABasePlayerWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ABasePlayerWeapon();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TObjectPtr<UWeaponItemDataAsset> ItemDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TObjectPtr<UAnimMontage> ItemMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FPlayerWeaponData PlayerWeaponData;

	// 콜리전이 붙을 SkeletalMesh의 소켓, 또는 본 네임
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FName CollisionSocketName;

	// 장착x 소지했을 때의 무기 위치 소켓이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CarriedSocketName;
	// 왼손에 장착했을 때 소켓이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquippedLSocketName;
	// 오른손에 장착했을 때 소켓이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquippedRSocketName;

	// 무기데미지 커브테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Damage;

	// 플레이어에게 주입된 무기능력 초기화
	UFUNCTION()
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);

	// 플레이어에게 주입된 무기능력 반환
	UFUNCTION()
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
	
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UWeaponItemDataAsset* GetWeaponData() const { return ItemDataAsset; }
	// 무기 Material Instance 조정
	UFUNCTION()
	void DissolveFXWeapon(bool bDissolve);

	UPROPERTY()
	UCurveFloat* CurveFloat;
	
	bool bIsDissolving = false;
	float DissolveTimeElapsed = 0.f;
	float DissolveDuration = 0.7f;	

public:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
