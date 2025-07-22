// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EneAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"


AEneAIController::AEneAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	
//팀 아이디 부여
	AAIController::SetGenericTeamId(FGenericTeamId(1));
	
	//AISenseConfig_Sight 생성자
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	
	//적 감지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;

	//아군 감지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;

	//중립 감지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	//시아 반경 설정
	AISenseConfig_Sight->SightRadius = 3000.0f;

	//대상을 잃는 시아 설정
	AISenseConfig_Sight->LoseSightRadius = 0.f;

	//주변 시아각
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 230.0f;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	//센서설정
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AEneAIController::OnEnemyPerceptionUpdated);
	AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));
	
	
	
}

void AEneAIController::BeginPlay()
{
	Super::BeginPlay();
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdFollowingComponent->SetCrowdSimulationState(bDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		default:break;
		}

	
		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);

		if (BehaviorTreeAsset)
		{
			RunBehaviorTree(BehaviorTreeAsset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BeginPlay: BehaviorTreeAsset is null"));
		}
	}
}

void AEneAIController::HandleDamageStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("[DAMAGE] Took damage from: %s"), *Actor->GetName());

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleDamageStimulus: ControlledPawn is null"));
		return;
	}
	// 데미지 방향으로 즉시 회전
	FVector DamageDirection = (Actor->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
	FRotator LookRotation = DamageDirection.Rotation();
	GetPawn()->SetActorRotation(LookRotation);
}
//
// void AEneAIController::AdjustPerceptionForState(EAIState State)
// {
// 	if (!AISenseConfig_Sight ) return;
// 	switch (State)
// 	{
// 	case EAIState::Alert:
// 		AISenseConfig_Sight->SightRadius = 1800.0f;
// 	
// 		break;
//         
// 	case EAIState::Combat:
// 		AISenseConfig_Sight->SightRadius = 2000.0f;
// 		
// 		break;
//         
// 	case EAIState::Investigate:
// 		AISenseConfig_Sight->SightRadius = 1200.0f;
// 		
// 		break;
//         
// 	default:
// 		// 기본 설정으로 복원
// 		AISenseConfig_Sight->SightRadius = 1500.0f;
// 	
// 		break;
// 	}
// 	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
// }

void AEneAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//Update
	//Debug::Print(Actor->GetActorLabel() + TEXT(" Was Sensed"), FColor::Green);

	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
	if (Attitude != ETeamAttitude::Hostile)
	{
		return; // 아군이나 중립이면 무시
	}
	//인지
	 if (Stimulus.WasSuccessfullySensed() && Actor)
	 {
	 	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	 	{
	 		//TargetActor가 설정이 되있지 않으면 설정을 해라
	 		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
	 		{
	 			BlackboardComponent->SetValueAsObject(FName(TEXT("TargetActor")), Actor);
	 		}
	 	}
	 }
}

ETeamAttitude::Type AEneAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if (!OtherPawn) return ETeamAttitude::Neutral;

	const AController* OtherController = OtherPawn->GetController();
	if (!OtherController) return ETeamAttitude::Neutral;

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherController);
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	if (OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

