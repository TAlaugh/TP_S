// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "PreviewPlayerCharacter.generated.h"

class ABaseEnemyCharacter;
class UCameraComponent;
class USpringArmComponent;
class ABasePlayerCharacter;

UCLASS()
class TP_S_API APreviewPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APreviewPlayerCharacter();

	UFUNCTION()
	void CopyMeshFromPlayer(ABasePlayerCharacter* PlayerCharacter);

	UFUNCTION()
	void AttachWeaponMeshComponent(USkeletalMesh* WeaponMesh, FName SocketName = TEXT("hand_rPreviewSocket"));

	UFUNCTION()
	void PlayEquipMontage(FGameplayTag WeaponTag);
	
	UFUNCTION()
	void PlayEnterMontage();

protected:
	virtual void BeginPlay() override;

private:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	// USpringArmComponent* CameraBoom;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	// UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TMap<FGameplayTag, UAnimMontage*> EquipMontageMap;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* PreviewEnterMontage;
	
	UPROPERTY()
	USkeletalMeshComponent* PreviewWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Preview", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY()
	ABasePlayerCharacter* Player;
};
