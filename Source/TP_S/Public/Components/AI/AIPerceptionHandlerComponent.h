// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Components/ActorComponent.h"
// #include "Perception/AIPerceptionTypes.h"
// #include "Perception/AIPerceptionComponent.h" 
// #include "AIPerceptionHandlerComponent.generated.h"
//
// struct FAIStimulus;
// class UAISenseConfig_Sight;
// class UAISenseConfig_Hearing;
// class UAISenseConfig_Damage;
// class UAISenseConfig_Team;
//
// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
// class TP_S_API UAIPerceptionHandlerComponent : public UActorComponent
// {
// 	GENERATED_BODY()
//
// public:	
// 	// Sets default values for this component's properties
// 	UAIPerceptionHandlerComponent();
//
// protected:
// 	// Called when the game starts
// 	virtual void BeginPlay() override;
//
// 	//AI 컴포넌트들
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Components")
// 	UAIPerceptionComponent* AIPerceptionComponent;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Components")
// 	UAISenseConfig_Sight* AISenseConfig_Sight;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Components")
// 	UAISenseConfig_Hearing* AISenseConfig_Hearing;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Components")
// 	UAISenseConfig_Damage* AISenseConfig_Damage;
//
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI|Components")
// 	UAISenseConfig_Team* AISenseConfig_Team;
//
// 	// 가상함수들
// 	
// 	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
//
//
//
// 	//감각별 처리 함수들
// 	UFUNCTION(BlueprintCallable, Category="AI|Perception")
// 	void HandleSightStimulus(AActor* Actor, FAIStimulus Stimulus);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Perception")
// 	void HandleHearingStimulus(AActor* Actor, FAIStimulus Stimulus);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Perception")
// 	void HandleDamageStimulus(AActor* Actor, FAIStimulus Stimulus);
//     
// 	UFUNCTION(BlueprintCallable, Category="AI|Perception")
// 	void HandleTeamStimulus(AActor* Actor, FAIStimulus Stimulus);
//
//
// public:	
// 	// Called every frame
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//
// 	
// 	//Sense ID
// 	FAISenseID SightSenseID;
// 	FAISenseID HearingSenseID;
// 	FAISenseID DamageSenseID;
// 	FAISenseID TeamSenseID;
// };
