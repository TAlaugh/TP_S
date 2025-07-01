// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetBase.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}


void UWidgetBase::InitEnemyCreateWidget(AActor* EnemyActor)
{
	
	if (IBaseUIInterface* PawnUIInterface = Cast<IBaseUIInterface>(EnemyActor))
	{
		if (UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent())
		{
			BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
		}
	}
}