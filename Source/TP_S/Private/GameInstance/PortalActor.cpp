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
				
				// GetWorld()->ServerTravel("/Game/HeavyAssets/MainMenuAsset/MainMenu?listen", true);
				GetWorld()->ServerTravel("/Game/HeavyAssets/Boss06_Map?listen", true);
			}
		}
	}
}
