#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "AT_Attack_Throw_Weapon.generated.h"

class ABasePlayerWeapon;
class ABasePlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowWeaponTaskCompleted);

/**
 * 
 */
UCLASS()
class TP_S_API UAT_Attack_Throw_Weapon : public UAbilityTask_MoveToLocation
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FMoveToLocationDelegate OnFinished;
	UPROPERTY()
	ABasePlayerCharacter* Owner;

	UPROPERTY()
	FVector OwnerLocation;
	
	UPROPERTY()
	ABasePlayerWeapon* Weapon;

	UPROPERTY()
	AActor* Target;
	
	FVector WeaponLocation;

	UPROPERTY()
	bool bHasArrived = false; 

	float InterpSpeed;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Throw Weapon To Location",  HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAT_Attack_Throw_Weapon* ThrowWeaponToLocation(UGameplayAbility* OwningAbility, AActor* ActorToMove, FVector Destination, float InInterpSpeed = 500.0f);
	
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
};
