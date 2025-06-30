// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BasePlayerAnimInstance.h"

#include "DebugHelper.h"
#include "Chacracter/Player/BasePlayerCharacter.h"

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter != nullptr)
	{
		OwningPlayerCharacter = Cast<ABasePlayerCharacter>(OwningCharacter); 
	}
}

void UBasePlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		FootPhase = GetCurveValue("FootPhase");		
	} else
	{
		FootPhase = 0;
	}
	
	//Debug::Print(TEXT("%f"), FootPhase);
}
