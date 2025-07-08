// Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "Components/AI/AICommunicationComponent.h"
//
// #include "Controllers/EneAIController.h"
// #include "Kismet/GameplayStatics.h"
//
// class UBlackboardComponent;
// class UBehaviorTreeComponent;
//
// // Sets default values for this component's properties
// UAICommunicationComponent::UAICommunicationComponent()
// {
// 	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// 	// off to improve performance if you don't need them.
// 	PrimaryComponentTick.bCanEverTick = true;
//
// 	// ...
// }
//
//
// // Called when the game starts
// void UAICommunicationComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// ...
// 	
// }
//
// void UAICommunicationComponent::CallForBackup(AActor* Threat)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Calling for backup against: %s"), *Threat->GetName());
//
// 	TArray<AActor*> AlliedActors;
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GetPawn()->GetClass(), AlliedActors);
//     
// 	for (AActor* Allied : AlliedActors)
// 	{
// 		if (Allied != GetPawn())
// 		{
// 			float Distance = FVector::Dist(Allied->GetActorLocation(), GetPawn()->GetActorLocation());
// 			if (Distance < 1000.0f) // 1000 유닛 내 아군
// 			{
// 				if (AEneAIController* AlliedController = Cast<AEneAIController>(Allied->GetInstigatorController()))
// 				{
// 					AlliedController->ReceiveBackupRequest(GetPawn(), Threat);
// 				}
// 			}
// 		}
// 	}
// }
//
// void UAICommunicationComponent::ReceiveBackupRequest(APawn* Requester, AActor* Threat)
// {
// 	// 현재 상태에 따라 도움 여부 결정
// 	EAIState CurrentState = GetCurrentAIState();
// 	if (CurrentState == EAIState::Idle || CurrentState == EAIState::Patrol)
// 	{
// 		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Threat);
// 		GetBlackboardComponent()->SetValueAsObject(TEXT("HelpTarget"), Requester);
// 		SetAIState(EAIState::Alert);
// 	}
// }
//
// void UAICommunicationComponent::ShareCurrentTargetInfo(AActor* AlliedActor)
// {
// 	AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
// 	if (CurrentTarget)
// 	{
// 		if (AEneAIController* AlliedController = Cast<AEneAIController>(AlliedActor->GetInstigatorController()))
// 		{
// 			AlliedController->ReceiveTargetInfo(CurrentTarget, GetPawn()->GetActorLocation());
// 		}
// 	}
// }
//
// void UAICommunicationComponent::ReceiveTargetInfo(AActor* Target, FVector LastKnownLocation)
// {
// 	UE_LOG(LogTemp, Log, TEXT("Received target info: %s at %s"), 
// 	   *Target->GetName(), *LastKnownLocation.ToString());
//
// 	if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
// 	{
// 		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Target);
// 		GetBlackboardComponent()->SetValueAsVector(TEXT("InvestigateLocation"), LastKnownLocation);
// 		SetAIState(EAIState::Investigate);
// 	}
// }
//
//
// // Called every frame
// void UAICommunicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
//
