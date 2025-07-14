// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Enemy/EnemyHealthBar.h"

#include "Components/ProgressBar.h"
#include "Components/UI/EnemyUIcomponent.h"

void UEnemyHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (UEnemyUIComponent* EnemyUI = Cast<UEnemyUIComponent>(GetOwningPlayerPawn()->GetComponentByClass(UEnemyUIComponent::StaticClass())))
	{
		EnemyUI->OnCurrentHpChanged.AddDynamic(this, &UEnemyHealthBar::OnCurrentHPChanged);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEnemyHealthBar::OnCurrentHPChanged(float NewPercent)
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimerHandle);

	if (ProgressBar_HP)
	{
		ProgressBar_HP->SetPercent(NewPercent);
	}

	const bool bShouldHide = FMath::IsNearlyEqual(NewPercent, 0.0f) || bIsFirst;

	SetVisibility(bShouldHide ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (!bShouldHide)
	{
		GetWorld()->GetTimerManager().SetTimer(
			HealthTimerHandle,
			this,
			&UEnemyHealthBar::HiddenHealthBar,
			5.f
		);
	}

	bIsFirst = false;
}
 
void UEnemyHealthBar::HiddenHealthBar()
{
	SetVisibility(ESlateVisibility::Hidden);
}
