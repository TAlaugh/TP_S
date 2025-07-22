// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseType/BaseEnumType.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "BaseFunctionLibrary.generated.h"

class UBaseCombatComponent;
struct FGameplayTag;
class UBaseAbilitySystemComponent;
class UBaseCombatComponent;
struct FScalableFloat;

/**
 * 
 */
UCLASS()
class TP_S_API UBaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UBaseAbilitySystemComponent* NativeGetBaseASCFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);
	
	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EBaseConfirmType& OutConfirmType);
	
	static UBaseCombatComponent* NativeGetBaseCombatComponentFromActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (DisplayName = "Get Base Combat Component From Actor",  ExpandEnumAsExecs = "OutValidType"))
	static UBaseCombatComponent* BP_GetBaseCombatComponentFromActor(AActor* Actor, EBaseValidType& OutValidType);

	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (DisplayName = "Get Enemy Combat Component From Actor",  ExpandEnumAsExecs = "OutValidType"))
	static UEnemyCombatComponent* BP_GetEnemyCombatComponentFromActor(AActor* Actor, EBaseValidType& OutValidType);
	
	UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);


	UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToActor(AActor* InInstigator,AActor* InTargetActor, const FGameplayEffectSpecHandle InSpecHandle);
	/*
	UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);
	
	UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);
	
	UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);
	*/

	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject,float TotalTime,float UpdateInterval,
		float& OutRemainingTime,EBaseCountDownActionInput CountDownInput,
		UPARAM(DisplayName = "Output")EBaseCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);
};
