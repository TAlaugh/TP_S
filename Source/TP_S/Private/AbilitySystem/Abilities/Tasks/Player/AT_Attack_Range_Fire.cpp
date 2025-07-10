// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Range_Fire.h"

#include "DebugHelper.h"

UAT_Attack_Range_Fire* UAT_Attack_Range_Fire::Action(UGameplayAbility* OwningAbility, float Interval)
{
	UAT_Attack_Range_Fire* Task = NewAbilityTask<UAT_Attack_Range_Fire>(OwningAbility);
	Task->ActionInterval = Interval;
	
	return Task;
}

void UAT_Attack_Range_Fire::Activate()
{
	Super::Activate();

	StartFire();

	// 타이머로 ActionInterval마다 계속 StartFire 실행
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ThisClass::StartFire, ActionInterval, true);

}

void UAT_Attack_Range_Fire::OnDestroy(bool bInOwnerFinished)
{
	StopFire();
	Super::OnDestroy(bInOwnerFinished);
}

void UAT_Attack_Range_Fire::StopFire()
{
	if (Timer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		OnFinishedTask.Broadcast();
		EndTask();
	}
}

void UAT_Attack_Range_Fire::StartFire()
{
	OnStartedTask.Broadcast();
}
