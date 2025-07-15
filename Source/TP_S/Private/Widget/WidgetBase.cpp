// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetBase.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UWidgetBase::InitEnemyCreateWidget(AActor* OwningEnemyActor)
{
	if (IBaseUIInterface* PawnUIInterface = Cast<IBaseUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent,TEXT("Failed to extrac an EnemyUIComponent from %s"),*OwningEnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}