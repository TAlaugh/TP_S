// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseType/BaseEnumType.h"
#include "BaseFunctionLibrary.generated.h"

struct FGameplayTag;
class UBaseAbilitySystemComponent;
struct FScalableFloat;

/**
 * 
 */
UCLASS()
class TP_S_API UBaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UBaseAbilitySystemComponent* NativeGetBaseASCFromActor(AActor*  InActor);

	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EBaseConfirmType& OutConfirmType);

	// static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	//
	// UFUNCTION(BlueprintCallable, Category = "Base|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor",  ExpandEnumAsExecs = "OutValidType"))
	// static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EBaseValidType& OutValidType);

	// UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	// static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);
	//
	// UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	// static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);
	//
	// UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	// static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);
	//
	// UFUNCTION(BlueprintPure, Category = "Base|FunctionLibrary")
	// static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);
	//
};
