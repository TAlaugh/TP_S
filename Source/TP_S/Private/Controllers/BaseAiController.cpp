// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BaseAiController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AIPerceptionTypes.h" 
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"
#include "Perception/AISense.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"


ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		
	}

	CurrentHealth = MaxHealth;
	CurrentHealthState = EHealthState::Healthy;
	
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

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ABaseAIController::OnEnemyPerceptionUpdated);
	SetPerceptionComponent(*AIPerceptionComponent);
	
}


void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
			this, &ABaseAIController::OnEnemyPerceptionUpdated);
	}

	SightSenseID = UAISense_Sight::StaticClass()->GetDefaultObject<UAISense_Sight>()->GetSenseID();
	HearingSenseID = UAISense_Hearing::StaticClass()->GetDefaultObject<UAISense_Hearing>()->GetSenseID();
	DamageSenseID = UAISense_Damage::StaticClass()->GetDefaultObject<UAISense_Damage>()->GetSenseID();
	TeamSenseID = UAISense_Team::StaticClass()->GetDefaultObject<UAISense_Team>()->GetSenseID();
}


void ABaseAIController::HandleLoseSight(AActor* LostActor)
{
	// 블랙보드 초기화나 상태 전환
	GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
	UE_LOG(LogTemp, Log, TEXT("Lost sight of %s and cleared target."), *LostActor->GetName());
}


void ABaseAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	// FAISenseID를 직접 비교
	FAISenseID SenseID = Stimulus.Type;
    
	if (SenseID == SightSenseID)
	{
		HandleSightStimulus(Actor, Stimulus);
	}
	else if (SenseID == HearingSenseID)
	{
		HandleHearingStimulus(Actor, Stimulus);
	}
	else if (SenseID == DamageSenseID)
	{
		HandleDamageStimulus(Actor, Stimulus);
	}
	else if (SenseID == TeamSenseID)
	{
		HandleTeamStimulus(Actor, Stimulus);
	}
}

void ABaseAIController::HandleSightStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Log, TEXT("[SIGHT] Actor seen: %s"), *Actor->GetName());
        
		// 시각적 감지 시에만 타겟 설정
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
        
		// 거리별 상태 차별화
		const float Distance = FVector::Dist(Actor->GetActorLocation(), GetPawn()->GetActorLocation());
        
		if (Distance < 300.f)
		{
			SetAIState(EAIState::Combat);
			UE_LOG(LogTemp, Warning, TEXT("COMBAT RANGE: %s at distance %.2f"), *Actor->GetName(), Distance);
		}
		else if (Distance < 800.f)
		{
			SetAIState(EAIState::Alert);
			UE_LOG(LogTemp, Log, TEXT("ALERT RANGE: %s at distance %.2f"), *Actor->GetName(), Distance);
		}
		else
		{
			SetAIState(EAIState::Investigate);
			UE_LOG(LogTemp, Log, TEXT("INVESTIGATE RANGE: %s at distance %.2f"), *Actor->GetName(), Distance);
		}
		// 시야 상실 타이머 정리
		GetWorld()->GetTimerManager().ClearTimer(LoseSightTimerHandle);
		}
		else
		{
		UE_LOG(LogTemp, Log, TEXT("[SIGHT] Actor lost: %s"), *Actor->GetName());
        
			// 시야 상실 시에만 타이머 설정
			GetWorld()->GetTimerManager().SetTimer(
				LoseSightTimerHandle,
				FTimerDelegate::CreateUObject(this, &ABaseAIController::HandleLoseSight, Actor),
				3.0f, false);
		}
	}

void ABaseAIController::HandleHearingStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Log, TEXT("[HEARING] Heard sound from: %s at %s"),
			*Actor->GetName(), *Stimulus.StimulusLocation.ToString());
        
		// 청각 감지 시 조사 상태로 전환
		GetBlackboardComponent()->SetValueAsVector(TEXT("InvestigateLocation"), Stimulus.StimulusLocation);
        
		EAIState CurrentState = GetCurrentAIState();
		if (CurrentState == EAIState::Idle || CurrentState == EAIState::Patrol)
		{
			SetAIState(EAIState::Investigate);
		}
        
		// 소리 방향으로 시선 조정
		FVector DirectionToSound = (Stimulus.StimulusLocation - GetPawn()->GetActorLocation()).GetSafeNormal();
		FRotator LookRotation = DirectionToSound.Rotation();
		GetPawn()->SetActorRotation(FMath::RInterpTo(GetPawn()->GetActorRotation(), LookRotation, GetWorld()->GetDeltaSeconds(), 2.0f));
	}
}

void ABaseAIController::HandleDamageStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("[DAMAGE] Took damage from: %s"), *Actor->GetName());
    
	// 데미지 받은 경우 즉시 전투 상태로 전환
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
	SetAIState(EAIState::Combat);
	
	// 데미지 방향으로 즉시 회전
	FVector DamageDirection = (Actor->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
	FRotator LookRotation = DamageDirection.Rotation();
	GetPawn()->SetActorRotation(LookRotation);
}

void ABaseAIController::HandleTeamStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("[TEAM] Team change or detection: %s"), *Actor->GetName());
    
	// 팀 관계 재평가
	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
    
	switch (Attitude)
	{
	case ETeamAttitude::Friendly:
		// 아군 발견 시 정보 공유
		ShareCurrentTargetInfo(Actor);
		break;
        
	case ETeamAttitude::Hostile:
		// 적군 발견 시 경계 태세
		if (GetCurrentAIState() == EAIState::Idle)
		{
			SetAIState(EAIState::Alert);
		}
		break;
        
	case ETeamAttitude::Neutral:
		// 중립 대상은 관찰만
		break;
	}
}

void ABaseAIController::UpdateLastKnownLocation(AActor* Actor, FVector Location)
{
	if (Actor)
	{
		LastKnownLocations.Add(Actor, Location);
		LastSeenTimes.Add(Actor,GetWorld()->GetTimeSeconds());
	}
}

FVector ABaseAIController::GetLastKnownLocation(AActor* Actor) const
{
	if (const FVector* Location = LastKnownLocations.Find(Actor))
	{
		return *Location;
	}
	return FVector::ZeroVector;
}

void ABaseAIController::ClearActorMemory(AActor* Actor)
{
	LastKnownLocations.Remove(Actor);
	LastSeenTimes.Remove(Actor);
}

void ABaseAIController::SetAIState(EAIState NewState)
{
	EAIState CurrentState = GetCurrentAIState();
	if (CurrentState != NewState)
	{
		OnStateExit(CurrentState);
		GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(NewState));
		OnStateEnter(NewState);
        
		UE_LOG(LogTemp, Log, TEXT("AI State changed from %d to %d"), static_cast<int32>(CurrentState), static_cast<int32>(NewState));
	}
}

EAIState ABaseAIController::GetCurrentAIState() const
{
	return static_cast<EAIState>(GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState")));
}

void ABaseAIController::OnStateEnter(EAIState State)
{
	switch (State)
	{
	case EAIState::Alert:
		// 감지 범위 증가
		AdjustPerceptionForState(State);
		break;
        
	case EAIState::Combat:
		// 최대 감지 능력, 공격 준비
		AdjustPerceptionForState(State);
		break;
        
	case EAIState::Investigate:
		// 청각 민감도 증가
		AdjustPerceptionForState(State);
		break;
	}
}

void ABaseAIController::OnStateExit(EAIState State)
{
}

void ABaseAIController::CallForBackup(AActor* Threat)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling for backup against: %s"), *Threat->GetName());
    
	// 주변 아군에게 알림 (실제 구현에서는 Communication Manager 사용)
	TArray<AActor*> AlliedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GetPawn()->GetClass(), AlliedActors);
    
	for (AActor* Allied : AlliedActors)
	{
		if (Allied != GetPawn())
		{
			float Distance = FVector::Dist(Allied->GetActorLocation(), GetPawn()->GetActorLocation());
			if (Distance < 1000.0f) // 1000 유닛 내 아군
			{
				if (ABaseAIController* AlliedController = Cast<ABaseAIController>(Allied->GetInstigatorController()))
				{
					AlliedController->ReceiveBackupRequest(GetPawn(), Threat);
				}
			}
		}
	}
}

void ABaseAIController::ReceiveBackupRequest(APawn* Requester, AActor* Threat)
{
	
	UE_LOG(LogTemp, Log, TEXT("Received backup request from %s against %s"), 
		*Requester->GetName(), *Threat->GetName());
    
	// 현재 상태에 따라 도움 여부 결정
	EAIState CurrentState = GetCurrentAIState();
	if (CurrentState == EAIState::Idle || CurrentState == EAIState::Patrol)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Threat);
		GetBlackboardComponent()->SetValueAsObject(TEXT("HelpTarget"), Requester);
		SetAIState(EAIState::Alert);
	}
}

void ABaseAIController::ShareCurrentTargetInfo(AActor* AlliedActor)
{
	AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (CurrentTarget)
	{
		if (ABaseAIController* AlliedController = Cast<ABaseAIController>(AlliedActor->GetInstigatorController()))
		{
			AlliedController->ReceiveTargetInfo(CurrentTarget, GetPawn()->GetActorLocation());
		}
	}
}

void ABaseAIController::ReceiveTargetInfo(AActor* Target, FVector LastKnownLocation)
{
	UE_LOG(LogTemp, Log, TEXT("Received target info: %s at %s"), 
	   *Target->GetName(), *LastKnownLocation.ToString());
    
	// 현재 타겟이 없으면 새로운 타겟 설정
	if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Target);
		GetBlackboardComponent()->SetValueAsVector(TEXT("InvestigateLocation"), LastKnownLocation);
		SetAIState(EAIState::Investigate);
	}
}


void ABaseAIController::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = FMath::Max(1.0f, NewMaxHealth);
    
	// 현재 체력이 최대 체력보다 크면 조정
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
    
	UpdateHealthState();
}

void ABaseAIController::SetCurrentHealth(float NewHealth)
{
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    
	UpdateHealthState();
    
	// 체력 감소 시 반응
	if (CurrentHealth < OldHealth)
	{
		OnHealthDecreased(OldHealth - CurrentHealth);
	}
}

void ABaseAIController::ModifyHealth(float Delta)
{
	SetCurrentHealth(CurrentHealth + Delta);
}

float ABaseAIController::GetHealthPercentage() const
{
	if (MaxHealth <= 0.0f) return 0.0f;
	return (CurrentHealth / MaxHealth) * 100.0f;
}

void ABaseAIController::UpdateHealthState()
{
	EHealthState OldState = CurrentHealthState;
	float HealthPercent = GetHealthPercentage();
    
	// 체력 상태 결정
	if (HealthPercent >= InjuredThreshold)
	{
		CurrentHealthState = EHealthState::Healthy;
	}
	else if (HealthPercent >= CriticalThreshold)
	{
		CurrentHealthState = EHealthState::Injured;
	}
	else
	{
		CurrentHealthState = EHealthState::Critical;
	}
    
	// 상태가 변경되었으면 반응
	if (OldState != CurrentHealthState)
	{
		OnHealthStateChanged(CurrentHealthState);
		HandleLowHealthReaction();
	}
}

void ABaseAIController::OnHealthStateChanged(EHealthState NewState)
{
	UE_LOG(LogTemp, Warning, TEXT("AI %s health state changed to %d (%.1f%% health)"), 
		   *GetPawn()->GetName(), (int32)NewState, GetHealthPercentage());
    
	switch (NewState)
	{
	case EHealthState::Healthy:
		OnHealthy();
		break;
	case EHealthState::Injured:
		OnInjured();
		break;
	case EHealthState::Critical:
		OnCritical();
		break;
	}
	OnHealthStateChangedEvent(NewState, GetHealthPercentage());
}

void ABaseAIController::HandleLowHealthReaction()
{
	if (CurrentHealthState == EHealthState::Healthy)
		return;
	switch (PersonalityType)
	{
	case EAIPersonality::Aggressive:
		HandleAggressiveLowHealth();
		break;
        
	case EAIPersonality::Cowardly:
		HandleCowardlyLowHealth();
		break;
        
	case EAIPersonality::Berserker:
		HandleBerserkerLowHealth();
		break;
   
	}
}

void ABaseAIController::HandleAggressiveLowHealth()
{
	switch (CurrentHealthState)
	{
	case EHealthState::Injured:
		// 부상 상태: 더 공격적으로 변함
		if (AISenseConfig_Sight)
		{
			AISenseConfig_Sight->SightRadius *= 1.2f;
		}
		SetAIState(EAIState::Alert);
		break;
		
	case EHealthState::Critical:
		// 위험 상태: 최후의 발악
		SetAIState(EAIState::Combat);
		OnPersonalityHealthReaction(TEXT("LAST_STAND"));
		break;
	}
}

void ABaseAIController::HandleCowardlyLowHealth()
{
	switch (CurrentHealthState)
	{
	case EHealthState::Injured:
		// 부상 상태: 즉시 도망
		SetAIState(EAIState::Flee);
		break;
	
	case EHealthState::Critical:
		// 중상/위험 상태: 패닉 상태로 도망
		SetAIState(EAIState::Flee);
		OnPersonalityHealthReaction(TEXT("PANIC_FLEE"));
		break;
	}
}

void ABaseAIController::HandleBerserkerLowHealth()
{
	switch (CurrentHealthState)
	{
	case EHealthState::Injured:
		// 부상 상태: 계속 전투
		SetAIState(EAIState::Combat);
		OnPersonalityHealthReaction(TEXT("IGNORE_PAIN"));
		break;
	
	case EHealthState::Critical:
		// 중상/위험 상태: 광전사 모드
		SetAIState(EAIState::Combat);
		if (AISenseConfig_Sight)
		{
			AISenseConfig_Sight->PeripheralVisionAngleDegrees = 45.0f; // 터널 비전
		}
		OnPersonalityHealthReaction(TEXT("BERSERKER_RAGE"));
		break;
	}
}

void ABaseAIController::OnHealthy()
{
	return;
}

void ABaseAIController::OnInjured()
{
	return;
}

void ABaseAIController::OnCritical()
{
	return;
}


void ABaseAIController::AdjustPerceptionForState(EAIState State)
{
	if (!AISenseConfig_Sight || !AISenseConfig_Hearing) return;
    
	switch (State)
	{
	case EAIState::Alert:
		AISenseConfig_Sight->SightRadius = 1800.0f;
		AISenseConfig_Hearing->HearingRange = 2500.0f;
		break;
        
	case EAIState::Combat:
		AISenseConfig_Sight->SightRadius = 2000.0f;
		AISenseConfig_Hearing->HearingRange = 3000.0f;
		break;
        
	case EAIState::Investigate:
		AISenseConfig_Sight->SightRadius = 1200.0f;
		AISenseConfig_Hearing->HearingRange = 2800.0f;
		break;
        
	default:
		// 기본 설정으로 복원
		AISenseConfig_Sight->SightRadius = 1500.0f;
		AISenseConfig_Hearing->HearingRange = 2000.0f;
		break;
	}
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
}

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	const FGenericTeamId MyTeamID = GetGenericTeamId();
	const FGenericTeamId OtherTeamID = OtherTeamAgent->GetGenericTeamId();

	if (MyTeamID == OtherTeamID)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}

