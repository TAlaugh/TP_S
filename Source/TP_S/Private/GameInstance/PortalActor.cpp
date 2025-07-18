// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PortalActor.h"

#include "Character/Player/BasePlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Controllers/BasePlayerController.h"
#include "GameInstance/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/BasePlayerState.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetBoxExtent(FVector(50.f));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->bHiddenInGame = false;
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnCollisionBoxBeginOverlap);
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalActor::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (ABasePlayerCharacter* PlayerChar = Cast<ABasePlayerCharacter>(OtherActor))
	// {
	// 	if (ABasePlayerState* PS = PlayerChar->GetPlayerState<ABasePlayerState>())
	// 	{
	// 		PS->CaptureFromComponents(PlayerChar);
	// 		PS->bShouldRestoreData = true;
	// 		UE_LOG(LogTemp, Warning, TEXT("[포탈] PlayerState에 컴포넌트 데이터 저장 완료"));
	// 	}
	// 	
	// 	if (!PlayerChar) return;
	// }
	//
	// // UE_LOG(LogTemp, Warning, TEXT("PlayerChar"));
	//
	// UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
	// if (!GI) return;
	//
	// if (GI->bBossStage)
	// {
	// 	GI->AdvancedFloor();
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("[포탈] 보스 처치! 다음 층으로 이동 → %d층"), GI->CurrentFloorCount);
	//
	// 	// TOOD : 층 맵 만들기
	// 	// const FString NextFloorName = FString::Printf(TEXT("Floor%d_Level"), GI->CurrentFloorCount);
	// 	// UGameplayStatics::OpenLevel(this, FName(*NextFloorName));
	// 	
	// 	// UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
	// 	
	// }
	// else
	// {
	// 	GI->AdvanceStage();
	//
	// 	if (GI->bBossStage)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("[포탈] 보스 스테이지 진입 예정 (다음 진입 시 보스 등장)"));
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("[포탈] 일반 스테이지 반복: StageCount %d / 3"), GI->CurrentStageCount);
	// 	}
	//
	// 	// UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
	// 	GetWorld()->ServerTravel("/Game/Content/Common/ThirdPerson/Maps/ThirdPersonMap?listen", true);
	// }

	if (ABasePlayerCharacter* PlayerChar = Cast<ABasePlayerCharacter>(OtherActor))
	{
		ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC && PC->GetPawn())
		{
			if (ABasePlayerState* PS = PlayerChar->GetPlayerState<ABasePlayerState>())
			{
				PS->CaptureFromComponents(PlayerChar);
				PS->bShouldRestoreData = true;
				UE_LOG(LogTemp, Warning, TEXT("[포탈] PlayerState에 컴포넌트 데이터 저장 완료 (Melee: %s)"),
					*PS->StoredCombatInfo.MeleeWeaponTag.ToString());
				
				UE_LOG(LogTemp, Warning, TEXT("bShouldRestoreData (Before Travel): %s"),
					PS->bShouldRestoreData ? TEXT("true") : TEXT("false"));
				
				GetWorld()->ServerTravel("Game/Content/Common/ThirdPerson/Maps/ThirdPersonMap?listen", true);
			}
		}
	}
}
