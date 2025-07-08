// Fill out your copyright notice in the Description page of Project Settings.
//
//  #pragma once
//
// UENUM(BlueprintType)
// enum class EAIState : uint8
// {
// 	Idle        UMETA(DisplayName = "Idle"),
// 	Patrol      UMETA(DisplayName = "Patrol"),
// 	Investigate UMETA(DisplayName = "Investigate"),
// 	Alert       UMETA(DisplayName = "Alert"),
// 	Combat      UMETA(DisplayName = "Combat"),
// 	Flee        UMETA(DisplayName = "Flee")
// };
//
// UENUM(BlueprintType)
// enum class EHealthState : uint8
// {
// 	Healthy     UMETA(DisplayName = "Healthy"),      // 건강함 (70%+)
// 	Injured     UMETA(DisplayName = "Injured"),      // 부상 (30-70%)
// 	Critical    UMETA(DisplayName = "Critical")      // 위험 (30% 이하)
// };
//
//
// #include "CoreMinimal.h"
// #include "AIController.h"
// #include "Perception/AIPerceptionTypes.h"
// #include "Perception/AIPerceptionComponent.h"  // 추가
// #include "Engine/TimerHandle.h"                // 추가 (FTimerHandle 사용)
// #include "GenericTeamAgentInterface.h"         // 추가 (팀 시스템)
// #include "BaseAiController.generated.h"
//
// struct FAIStimulus;
// class UAISenseConfig_Sight;
// class UAISenseConfig_Hearing;
// class UAISenseConfig_Damage;
// class UAISenseConfig_Team;
//
// class UBlackboardComponent;
// class UBehaviorTreeComponent;
// /**
//  * 
//  */
//
//
// UCLASS()
// class TP_S_API ABaseAIController : public AAIController
// {
// 	GENERATED_BODY()
//
// 	public:
// 	
// 	ABaseAIController(const FObjectInitializer& ObjectInitializer) ;
//
//
// protected:
// 	
// 	// 가상함수들
// 	
// 	virtual void HandleLoseSight(AActor* LostActor);
//
	// //감각별 처리 함수들
	// UFUNCTION(BlueprintCallable, Category="AI|Perception")
	// void HandleSightStimulus(AActor* Actor, FAIStimulus Stimulus);
 //    
	// UFUNCTION(BlueprintCallable, Category="AI|Perception")
	// void HandleDamageStimulus(AActor* Actor, FAIStimulus Stimulus);
 //   
// 	// //메모리 관리
// 	UFUNCTION(BlueprintCallable, Category="AI|Memory")
// 	void UpdateLastKnownLocation(AActor* Actor, FVector Location);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Memory")
// 	FVector GetLastKnownLocation(AActor* Actor) const;
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Memory")
// 	void ClearActorMemory(AActor* Actor);
//
// 	
// 	// 상태 관리
// 	
// 	UFUNCTION(BlueprintCallable, Category="AI|State")
// 	void SetAIState(EAIState NewState);
//     
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI|State")
// 	EAIState GetCurrentAIState() const;
// 	
// 	virtual void OnStateEnter(EAIState State);
// 	virtual void OnStateExit(EAIState State);
//     
// 	// 협력 시스템
// 	
// 	UFUNCTION(BlueprintCallable, Category="AI|Cooperation")
// 	void CallForBackup(AActor* Threat);
//     
// 	void ReceiveBackupRequest(APawn* Requester, AActor* Threat);
// 	void ShareCurrentTargetInfo(AActor* AlliedActor);
// 	void ReceiveTargetInfo(AActor* Target, FVector LastKnownLocation);
//
// 	// 유틸리티
// 	
// 	
// 	UFUNCTION(BlueprintImplementableEvent, Category="AI|Health")
// 	void OnHealthStateChangedEvent(EHealthState NewState, float HealthPercentage);
//
// 	UFUNCTION(BlueprintImplementableEvent, Category="AI|Health")
// 	void OnPersonalityHealthReaction(const FString& ReactionType);
//
// 	UFUNCTION(BlueprintImplementableEvent, Category="AI|Health")
// 	void OnHealthDecreased(float DamageAmount);
//
// 	
// 	// 체력 상태 관련 변수들
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Health")
// 	float MaxHealth = 100.0f;
//     
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Health")
// 	float CurrentHealth = 100.0f;
//     
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Health")
// 	EHealthState CurrentHealthState = EHealthState::Healthy;
//     
// 	// 체력 임계값들 (백분율)
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Health")
// 	float InjuredThreshold = 70.0f;
//     
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Health")
// 	float CriticalThreshold = 30.0f;
//
// 	// 체력 관련 함수들
// 	UFUNCTION(BlueprintCallable, Category="AI|Health")
// 	void SetMaxHealth(float NewMaxHealth);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Health")
// 	void SetCurrentHealth(float NewHealth);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Health")
// 	void ModifyHealth(float Delta);
//     
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI|Health")
// 	float GetHealthPercentage() const;
//
// 	UFUNCTION(BlueprintCallable, Category="AI|Health")
// 	void UpdateHealthState();
// 	
// 	UFUNCTION(BlueprintCallable, Category="AI|Health")
// 	void OnHealthStateChanged(EHealthState NewState);
// 	
// 	
// 	// 체력 상태별 처리 함수들
// 	virtual void OnHealthy();
// 	virtual void OnInjured();
// 	virtual void OnCritical();
// 	
// 	//감각들 미리 정해둔거
// 	void AdjustPerceptionForState(EAIState State);
//
//
// private:
//
// 	//상태 및 메모리
// 	
// 	 UPROPERTY()
// 	 float StressLevel = 0.0f;
// 	
// 	 UPROPERTY()
// 	 EAIState PreviousState = EAIState::Idle;
// 	
// 	 UPROPERTY()
// 	 FTimerHandle LoseSightTimerHandle;
// 	
// 	 UPROPERTY()
// 	 TMap<AActor*, FVector> LastKnownLocations;
//      
// 	 UPROPERTY()
// 	 TMap<AActor*, float> LastSeenTimes;
// 	
// 	//Crowd Avoidance 설정
// 	UPROPERTY(EditDefaultsOnly, Category="AI")
// 	bool bDetourCrowdAvoidance = true;
//
// 	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition="bDetourCrowdAvoidance", UIMin="1",UIMax="4"))
// 	int32 DetourCrowdAvoidanceQuality = 4;
// 	 
// 	UPROPERTY(EditDefaultsOnly, Category="AI", meta=(EditCondition="bDetourCrowdAvoidance"))
// 	float CollisionQueryRange = 600.0f;
//
//
// 	
// 	// === 팀 설정 ===
// 	UPROPERTY(EditDefaultsOnly, Category="AI|Team")
// 	FGenericTeamId TeamID = FGenericTeamId(0);
//
// 	//Sense ID
// 	FAISenseID SightSenseID;
// 	FAISenseID HearingSenseID;
// 	FAISenseID DamageSenseID;
// 	FAISenseID TeamSenseID;
// 	
// };
