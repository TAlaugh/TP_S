// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Consumables/BaseConsumableItem.h"

#include "DebugHelper.h"
#include "Components/SphereComponent.h"
#include "Components/Inventory/ConsumableInventoryComponent.h"
#include "Items/Consumables/ConsumableItem.h"

// Sets default values
ABaseConsumableItem::ABaseConsumableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	RootComponent = Mesh;
	
	Collision->SetupAttachment(Mesh);
	Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABaseConsumableItem::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap);
}

void ABaseConsumableItem::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;

	if (auto* ConsumableInventory = Pawn->FindComponentByClass<UConsumableInventoryComponent>())
	{
		ConsumableInventory->AddItem(ItemData, 1);
		Destroy();
	}
}
