// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetBaseASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));

}

void UBaseFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UBaseFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UBaseFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EBaseConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EBaseConfirmType::Yes : EBaseConfirmType::No;
}

// UPawnCombatComponent* UBaseFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
// {
// 	check(InActor);
//
// 	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
// 	{
// 		return PawnCombatInterface->GetPawnCombatComponent();
// 	}
// 	
// 	return nullptr;
// }
//
// UPawnCombatComponent* UBaseFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
// 	EBaseValidType& OutValidType)
// {
// 	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
//
// 	OutValidType = CombatComponent ? EBaseValidType::Valid : EBaseValidType::InValid;
//
// 	return CombatComponent;
// }

// bool UBaseFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
// {
// 	check(QueryPawn && TargetPawn);
// 	
// 	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
// 	IGenericTeamAgentInterface* TargetTeamAgent =Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
//
// 	if (QueryTeamAgent &&  TargetTeamAgent)
// 	{
// 		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
// 	}
// 	return false;
// }
//
// float UBaseFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
// {
// 	return InScalableFloat.GetValueAtLevel(InLevel);
// }

// FGameplayTag UBaseFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim,
// 	float& OutAngleDifference)
// {
// 	check(InAttacker && InVictim);
//
// 	const FVector VictimForward = InVictim->GetActorForwardVector();
// 	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();
//
// 	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
// 	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);
//
// 	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
//
// 	if (CrossResult.Z < 0.f)
// 	{
// 		OutAngleDifference *= -1.f;
// 	}
//
// 	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
// 	{
// 		return BaseGameplayTags::Shared_Status_HitReact_Front;
// 	}
// 	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
// 	{
// 		return BaseGameplayTags::Shared_Status_HitReact_Left;
// 	}
// 	else if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
// 	{
// 		return BaseGameplayTags::Shared_Status_HitReact_Back;
// 	}
// 	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
// 	{
// 		return BaseGameplayTags::Shared_Status_HitReact_Right;
// 	}
// 	return FGameplayTag();
// }

// bool UBaseFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
// {
// 	check(InAttacker && InDefender);
//
// 	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
//
// 	const FString DebugString = FString::Printf(TEXT("Dot Result : %f %s"), DotResult, DotResult < 0.f ? TEXT("Valid Block") : TEXT("InValid Block"));
// 	Debug::Print(DebugString, DotResult < 0.f ? FColor::Green : FColor::Red);
// 	
// 	return DotResult < -0.1f;
// }
