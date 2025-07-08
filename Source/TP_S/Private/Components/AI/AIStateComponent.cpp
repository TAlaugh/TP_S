// Fill out your copyright notice in the Description page of Project Settings.
//
//
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
// enum class EAIPersonality : uint8
// {
// 	Aggressive   UMETA(DisplayName = "Aggressive"),    // 공격적
// 	Berserker    UMETA(DisplayName = "Berserker"),     // 광전사
// 	Cowardly     UMETA(DisplayName = "Cowardly"),	   // 겁쟁이
// };
//
//
// #include "Components/AI/AIStateComponent.h"
//
// class UBehaviorTree;
// class UBlackboardComponent;
// class UBehaviorTreeComponent;
//
//
// UCLASS()
// class TP_S_API AAIStateComponent : public AAIStateComponent
// {
// 	GENERATED_BODY()
// 	public:
// 	AAIStateComponent(const FObjectInitializer& ObjectInitializer);
//
// protected:
//
//
// 	UPROPERTY(EditDefaultsOnly, Category = "AI")
// 	UBehaviorTree* BehaviorTreeAsset;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Personality")
// 	EAIPersonality PersonalityType = EAIPersonality::Aggressive;
// 	
// };
//
// // Sets default values for this component's properties
// UAIStateComponent::UAIStateComponent()
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
// void UAIStateComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// ...
// 	
// }
//
//
// // Called every frame
// void UAIStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
//
