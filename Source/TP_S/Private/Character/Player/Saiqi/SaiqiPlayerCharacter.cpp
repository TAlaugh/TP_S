// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_S/Public/Character/Player/Saiqi/SaiqiPlayerCharacter.h"

#include "Components/UI/BaseUIComponent.h"

ASaiqiPlayerCharacter::ASaiqiPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BaseUIComponent = CreateDefaultSubobject<UBaseUIComponent>(TEXT("BaseUIComponent"));
}

UBaseUIComponent* ASaiqiPlayerCharacter::GetBaseUIComponent() const
{
	return FindComponentByClass<UBaseUIComponent>();
}
