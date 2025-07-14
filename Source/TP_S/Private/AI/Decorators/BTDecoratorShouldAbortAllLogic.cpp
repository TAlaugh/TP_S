// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecoratorShouldAbortAllLogic.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecoratorShouldAbortAllLogic::UBTDecoratorShouldAbortAllLogic()
{
	NodeName = TEXT("Should Abort All Logic");
}

bool UBTDecoratorShouldAbortAllLogic::PerformConditionCheckAI(UBehaviorTreeComponent* OwnerComp,
	AAIController* OwnerController, APawn* ControlledPawn)
{
	if (!OwnerController || !ControlledPawn)
	{
		return false;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	// 대상 액터 가져오기
	UObject* ObjectValue = BlackboardComp->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ObjectValue);
	
	if (!IsValid(TargetActor))
	{
		return false;
	}

	// 타겟이 죽었는가?
	const bool bIsDeadTag = TargetActor->Tags.Contains(DeadTag);

	// AI 자신이 죽었는가?
	const bool bIsOwnerDead = ControlledPawn->Tags.Contains(DeadTag);

	// 거리 값이 0에 가까운가?
	const float DistanceValue = BlackboardComp->GetValueAsFloat(InDistToTargetKey.SelectedKeyName);
	const bool bIsDistanceZero = FMath::IsNearlyEqual(DistanceValue, 0.0f, 0.000001f);

	// 결과
	return bIsDeadTag || bIsOwnerDead || bIsDistanceZero;
}

