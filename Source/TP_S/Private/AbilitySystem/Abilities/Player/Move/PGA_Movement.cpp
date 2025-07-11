// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Move/PGA_Movement.h"

void UPGA_Movement::Action()
{
	if (MontageToPlay != nullptr)
	{
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				FName("None"),
				MontageToPlay
				);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		PlayMontageAndWait->ReadyForActivation();
	} else
	{
		OnCompleteCallback();
	}
}
