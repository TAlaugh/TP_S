#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "AT_Attack_Throw_Weapon.generated.h"

class ABasePlayerWeapon;
class ABasePlayerCharacter;
/**
 * 
 */
UCLASS()
class TP_S_API UAT_Attack_Throw_Weapon : public UAbilityTask_MoveToLocation
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY()
	ABasePlayerCharacter* Owner;
	
	UPROPERTY()
	ABasePlayerWeapon* Weapon;

	UPROPERTY()
	FVector WeaponLocation;

	UPROPERTY()
	float InterpSpeed;

	
};
