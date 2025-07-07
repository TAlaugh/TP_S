// // Fill out your copyright notice in the Description page of Project Settings.
// #include "Perception/AIPerceptionComponent.h"
// #include "Perception/AISenseConfig.h"
// #include "Perception/AIPerceptionTypes.h"
// #include "Perception/AISenseConfig_Sight.h"
// #include "Perception/AISenseConfig_Hearing.h"
// #include "Perception/AISenseConfig_Damage.h"
// #include "Perception/AISenseConfig_Team.h"
// #include "AIController.h"
// #include "Controllers/BaseAiController.h"
//
//
// #include "Components/AI/AIPerceptionHandlerComponent.h"
//
// // Sets default values for this component's properties
// UAIPerceptionHandlerComponent::UAIPerceptionHandlerComponent()
// {
// 	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// 	// off to improve performance if you don't need them.
// 	PrimaryComponentTick.bCanEverTick = true;
//
//
// 	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
// 	
// 	//AISenseConfig_Sight 생성자
// 	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
// 	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
// 	AISenseConfig_Sight->SightRadius = 1500;
// 	AISenseConfig_Sight->LoseSightRadius = 2000;
// 	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 160.0f;
// 	AISenseConfig_Sight->SetMaxAge(5.f);
//
// 	//AISenseConfig_Damage 생성자
// 	AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));
//
// 	//AISenseConfig_Hearing 생성자
// 	AISenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISenseConfig_Hearing"));
// 	AISenseConfig_Hearing->HearingRange = 2000;
// 	AISenseConfig_Hearing->SetMaxAge(10.f);
// 	AISenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
// 		
//
// 	//AISenseConfig_Team 생성자
// 	AISenseConfig_Team = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("AISenseConfig_Team"));
// 	AISenseConfig_Team -> SetMaxAge(2.f);
//
//
// 	//팀 아이디 부여
//
// 	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
// 	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Damage);
// 	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
// 	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Team);
// 	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
//
// 	//AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ABaseAIController::OnEnemyPerceptionUpdated);
// 	//SetPerceptionComponent(*AIPerceptionComponent);
// 	
// 	
// 	// ...
// }
//
//
// // Called when the game starts
// void UAIPerceptionHandlerComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// ...
// 	
// }
//
//
// void UAIPerceptionHandlerComponent::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
// {
// 	if (!Actor) return;
//
// 	// FAISenseID를 직접 비교
// 	FAISenseID SenseID = Stimulus.Type;
//     
// 	if (SenseID == SightSenseID)
// 	{
// 		HandleSightStimulus(Actor, Stimulus);
// 	}
// 	else if (SenseID == HearingSenseID)
// 	{
// 		HandleHearingStimulus(Actor, Stimulus);
// 	}
// 	else if (SenseID == DamageSenseID)
// 	{
// 		HandleDamageStimulus(Actor, Stimulus);
// 	}
// 	else if (SenseID == TeamSenseID)
// 	{
// 		HandleTeamStimulus(Actor, Stimulus);
// 	}
// }
//
// // Called every frame
// void UAIPerceptionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }

