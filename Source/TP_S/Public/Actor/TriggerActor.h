// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TriggerActor.generated.h"

UCLASS()
class TP_S_API ATriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger")
	UBoxComponent* TriggerBox;

	// ✅ 플레이어가 안에 있는지 여부
	bool bPlayerInside = false;

	// ✅ 현재 띄운 위젯 인스턴스
	UPROPERTY()
	UUserWidget* ActiveWidget;

	// ✅ UI 클래스 지정
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> StartWidgetClass;

	void ShowStartWidget();
	void HideStartWidget();

public:
	// ✅ 실제 상호작용 처리
	void Interact();

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

