// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DebugHelper.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "BaseInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class TP_S_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/*
	template<class UserObject, typename CallBackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func);
	*/

	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
		UserObject* ContextObject, CallbackFunc Func)
	{
		// InInputConfig Checked, and if It is null, print error)
		checkf(InInputConfig, TEXT("Input Config data asset is null, can not proceed with Binding"));

		if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
		{
			BindAction(FoundAction, TriggerEvent, ContextObject, Func);
		}
	}

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
		UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
	{
		checkf(InInputConfig, TEXT("Input Config data asset is null, can not proceed with Binding"))

		for (const FBaseInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
		{
			if (!AbilityInputActionConfig.IsValid()) continue;

			BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
			
			BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
			
		}
	}
};

/* 수업기반으로 재작성
template <class UserObject, typename CallBackFunc>
void UBaseInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func)
{
	check(InInputConfig);

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}
*/
