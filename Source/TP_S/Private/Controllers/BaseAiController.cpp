// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BaseAiController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"


ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		
	}

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	//AISenseConfig_Sight 생성자
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->SightRadius = 1500;
	AISenseConfig_Sight->LoseSightRadius = 2000;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 160.0f;
	AISenseConfig_Sight->SetMaxAge(5.f);

	//AISenseConfig_Damage 생성자
	AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));

	//AISenseConfig_Hearing 생성자
	AISenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISenseConfig_Hearing"));
	AISenseConfig_Hearing->HearingRange = 2000;
	AISenseConfig_Hearing->SetMaxAge(10.f);
	AISenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
		

	//AISenseConfig_Team 생성자
	AISenseConfig_Team = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("AISenseConfig_Team"));
	AISenseConfig_Team -> SetMaxAge(2.f);


	//팀 아이디 부여

	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Damage);
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Team);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ABaseAIController::OnEnemyPerceptionUpdated);
	SetPerceptionComponent(*AIPerceptionComponent);
	
}


void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
			this, &ABaseAIController::OnEnemyPerceptionUpdated);
	}
}

void ABaseAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)return;

	// FName SenseName = Stimulus.GetSense();
	//
	// if (SenseName == UAISense_Sight::StaticClass()->GetFName())
	// {
	// 	if (Stimulus.WasSuccessfullySensed())
	// 	{
	// 		UE_LOG(LogTemp,Log, TEXT("[SIGHT] Actor seen: %s"), *Actor->GetName());
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp,Log, TEXT("[SIGHT] Actor lost: %s"), *Actor->GetName());
	// 	}
	// }
	// else if (SenseName == UAISense_Hearing::StaticClass()->GetFName())
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[HEARING] Heard sound from: %s at %s"),
	// 		*Actor->GetName(), *Stimulus.StimulusLocation.ToString());
	// }
	// else if (SenseName == UAISense_Damage::StaticClass()->GetFName())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("[DAMAGE] Took damage from: %s"), *Actor->GetName());
	// 	// 예: 즉시 적으로 인식하고 공격 전환
	// }
	// else if (SenseName == UAISense_Team::StaticClass()->GetFName())
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("[TEAM] Team change or detection: %s"), *Actor->GetName());
	// }
}


ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Super::GetTeamAttitudeTowards(Other);
}

