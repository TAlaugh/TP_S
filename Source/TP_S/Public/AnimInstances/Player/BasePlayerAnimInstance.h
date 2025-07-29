// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "BaseType/Player/PlayerEnumType.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasePlayerAnimInstance.generated.h"

struct FFloatSpringState;
class UBaseAbilitySystemComponent;
class ABasePlayerCharacter;
/**
 * 
 */
UCLASS()
class TP_S_API UBasePlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UBasePlayerAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UBaseAbilitySystemComponent* ASC);
protected:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	/*
	UFUNCTION(BlueprintCallable, Category = "StateNodeFunction")
	void UpdateIdleState(FAnimUpdateContext& Context, FAnimNodeReference& Node) const;

	UFUNCTION(BlueprintCallable, Category = "StateNodeFunction")
	void SetUpStartState(FAnimUpdateContext& Context, FAnimNodeReference& Node) const;

	UFUNCTION(BlueprintCallable, Category = "StateNodeFunction")
	void UpdateStartState(FAnimUpdateContext& Context, FAnimNodeReference& Node) const;

	UFUNCTION(BlueprintCallable, Category = "StateNodeFunction")
	void UpdateStopState(FAnimUpdateContext& Context, FAnimNodeReference& Node) const;

	UFUNCTION(BlueprintCallable, Category = "StateNodeFunction")
	void UpdateLocomotionStateMachine(FAnimUpdateContext& Context, FAnimNodeReference& Node) const;
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	ABasePlayerCharacter* OwningPlayerCharacter;
	
	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterStateData")
	float GroundDistance = -1.0f;
	
	// 왼발이 앞에 있는지, 오른발이 앞에 있는지 : 왼발 = -1, 오른발 = 1
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FootPhase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bCrouch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bFalling;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float CachedLocomotionDirection;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	FRotator WorldRotation;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float YawDeltaSinceLastUpdate;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float AdditiveLeanAngle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float YawDeltaSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	FVector WorldLocation;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	float DisplacementSinceLastUpdate;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	float DisplacementSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	FVector WorldVelocity;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	FVector LocalVelocity2D;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	float LocalVelocityDirectionAngle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	float LocalVelocityDirectionAngleWidhtOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	EAnimEnumCardinalDirection LocalVelocityDirection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	EAnimEnumCardinalDirection LocalVelocityDirectionNoOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	bool bHasVelocity;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	FVector LocalAcceleration2D;
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	//bool bHasAcceleration;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	FVector PivotDirection2D;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bIsOnGround;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bIsCrouching;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bCrouchStateChange;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bADSStateChanged;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bWasADSLastUpdate;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	float TimeSinceFiredWeapon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bIsJumping;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bIsFalling;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool bIsRunningIntoWall;
	
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool bGameplayTag_IsADS;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool bGameplayTag_IsFiring;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool bGameplayTag_IsReloading;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool bGameplayTag_IsDashing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool bGameplayTag_IsMelee;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	EAnimEnumCardinalDirection StartDirection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	EAnimEnumCardinalDirection PivotInitialDirection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	float LastPivotTime;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	EAnimEnumCardinalDirection CardinalDirectionFromAcceleration;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "BlendWeightData")
	float UpperBodyDynamicAddtiveWeight;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AimingData")
	float AimYaw;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AimingData")
	float AimPitch;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Setting")
	float CardinalDirectionDeadZone;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LinkedLayerData")
	bool bLinkedLayerChanged;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LinkedLayerData")
	UAnimInstance* LastLinkedLayer;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float RootYawOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FFloatSpringState RootYawOffsetSpringState;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float TurnYawCurveValue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	EAnimEnum_RootYawOffsetMode RootYawOffsetMode;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FVector2D RootYawOffsetAngleClamp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FVector2D RootYawOffsetAngleClampCrouch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bIsFirstUpdate;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bEnableControlRig;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bUseFootPlacement;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bEnableRootYawOffset;
	

	
};
