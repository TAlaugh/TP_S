// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TP_S/Public/Character/BaseCharacter.h"
#include "BasePlayerCharacter.generated.h"


class UPlayerInventoryComponent;
class URangedWeaponInventoryComponent;
class UMeleeWeaponInventoryComponent;
class UBaseQuickSlotComponent;
class UConsumableInventoryComponent;
class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;


UCLASS()
class TP_S_API ABasePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
protected:
	ABasePlayerCharacter();

protected:
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
#pragma region Component
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta=(AllowPrivateAccess = "true"))
	UPlayerInventoryComponent* PlayerInventoryComponent;
	
#pragma endregion

#pragma region Input
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta=(AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(const FGameplayTag InputTag);
	void Input_AbilityInputReleased(const FGameplayTag InputTag);
	
#pragma endregion

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	int JumpCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	bool bAttackLight = false;
	
protected:
	void OpenInventory();
};
