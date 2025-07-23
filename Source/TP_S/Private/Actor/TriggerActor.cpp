// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TriggerActor.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/Boss06GameMode.h"

ATriggerActor::ATriggerActor()
{
    PrimaryActorTick.bCanEverTick = true;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATriggerActor::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::OnTriggerBeginOverlap);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerActor::OnTriggerEndOverlap);
}

void ATriggerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPlayerInside)
    {
        if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
        {
            if (PlayerController->WasInputKeyJustPressed(EKeys::F))
            {
                Interact();
            }
        }
    }
}

void ATriggerActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInside = true;
        ShowStartWidget();
    }
}

void ATriggerActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInside = false;
        HideStartWidget();
    }
}

void ATriggerActor::ShowStartWidget()
{
    if (!StartWidgetClass) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return;

    if (!ActiveWidget)
    {
        ActiveWidget = CreateWidget<UUserWidget>(PlayerController, StartWidgetClass);
        if (ActiveWidget)
        {
            ActiveWidget->AddToViewport();
        }
    }
}

void ATriggerActor::HideStartWidget()
{
    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }
}

void ATriggerActor::Interact()
{
    UE_LOG(LogTemp, Warning, TEXT("Stage Start Trigger Activated!"));

    // ✅ UI 제거
    HideStartWidget();

    // ✅ 현재 GameMode 가져오기
    if (ABoss06GameMode* BossGM = Cast<ABoss06GameMode>(UGameplayStatics::GetGameMode(this)))
    {
        BossGM->OnStageTriggerActivated();
        BossGM->StartStage();  
    }

    // ✅ 한 번만 사용되도록 트리거 제거
    Destroy();
}