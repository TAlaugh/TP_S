// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Boss06GameMode.h"
#include "Engine/AssetManager.h"
#include "Character/Enemy/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include  "DebugHelper.h"

void ABoss06GameMode::BeginPlay()
{
	Super::BeginPlay();

	// checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in 06Game mode Blueprint"))
	// SetCurrentBoss06GameMode(EBoss06GameModeState::WaitSpawnNewWave);
	//
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	//
	//PreLoadNextWaveEnemies();
}
void ABoss06GameMode::StartStage()
{
	CurrentWaveCount = 1;
	TotalSpawnedEnemiesThisWaveCounter = 0;
	CurrentSpawnedEnemiesCounter = 0;
	TimePassedSinceStart = 0.f;

	// ✅ 다음 웨이브 적 미리 로드
	PreLoadNextWaveEnemies();

	// ✅ GameMode 상태를 WaitSpawnNewWave로 전환 → Tick에서 자동으로 SpawnNewWave 진행
	SetCurrentBoss06GameMode(EBoss06GameModeState::WaitSpawnNewWave);

	UE_LOG(LogTemp, Warning, TEXT("Boss06GameMode::StartStage() called → Wave %d 준비"), CurrentWaveCount);
}

void ABoss06GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentBoss06GameModeState == EBoss06GameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentBoss06GameMode(EBoss06GameModeState::SpawnNewWave);
		}
	}
	if (CurrentBoss06GameModeState == EBoss06GameModeState::SpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;
			SetCurrentBoss06GameMode(EBoss06GameModeState::Inprogress);
			
		}
		
	}

	if (CurrentBoss06GameModeState == EBoss06GameModeState::WaveComplete)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;
			
			if (HasFinishedAllWaves())
			{
				SetCurrentBoss06GameMode(EBoss06GameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentBoss06GameMode(EBoss06GameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void ABoss06GameMode::SetCurrentBoss06GameMode(EBoss06GameModeState InState)
{
	CurrentBoss06GameModeState = InState;

	OnBoss06GameModeStateChanged.Broadcast(CurrentBoss06GameModeState);
	
}




bool ABoss06GameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void ABoss06GameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;	
	}

	PreLoadedEnemyClassMap.Empty();
	
	for (const FEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinition)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.LoadSynchronous();
		if (LoadedEnemyClass)
		{
			PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
			UE_LOG(LogTemp, Warning, TEXT("[Boss06] Loaded enemy class: %s"), *LoadedEnemyClass->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[Boss06] Failed to load enemy class: %s"), *SpawnerInfo.SoftEnemyClassToSpawn.ToString());
		}
		
	}
}

FBaseEnemyWaveSpawnerTableRow* ABoss06GameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FBaseEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FBaseEnemyWaveSpawnerTableRow>(RowName,FString());

	checkf(FoundRow,TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());

	return FoundRow;
}

int32 ABoss06GameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this,ATargetPoint::StaticClass(),TargetPointsArray);		
	}
	checkf(!TargetPointsArray.IsEmpty(),TEXT("No valid Target point found in level: %s for spawning enemies"),*GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;
	
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FBaseEnemyWaveSpawnerTableRow* WaveRow = GetCurrentWaveSpawnerTableRow();
	if (!WaveRow)
	{
		return 0;
	}

	for (const FEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinition)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())continue;
		UClass** FoundClass = PreLoadedEnemyClassMap.Find(SpawnerInfo.SoftEnemyClassToSpawn);
		if (!FoundClass)
		{
			UE_LOG(LogTemp, Error, TEXT("[Boss06] ❌ Enemy class not found in PreLoadedEnemyClassMap: %s"),
				*SpawnerInfo.SoftEnemyClassToSpawn.ToString());
			continue; // 맵에 없으면 스폰 시도 안 함
		}
		UClass* LoadedEnemyClass = *FoundClass;
		
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount,SpawnerInfo.MaxPerSpawnCount);

		//UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0,TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this,SpawnOrigin,RandomLocation,400.f);

			RandomLocation += FVector(0.f,0.f,150.f);

			ABaseEnemyCharacter*SpawnedEnemy =  GetWorld()->SpawnActor<ABaseEnemyCharacter>(LoadedEnemyClass,RandomLocation,SpawnRotation,SpawnParam);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}
			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}
	return EnemiesSpawnedThisTime;
}

bool ABoss06GameMode::ShouldKeepSpawnEnemies() const
{
	if (CurrentBoss06GameModeState != EBoss06GameModeState::Inprogress)
		return false;
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void ABoss06GameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{

	if (CurrentBoss06GameModeState != EBoss06GameModeState::Inprogress)
	{
		return;
	}
	
	CurrentSpawnedEnemiesCounter--;
	
	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	const int32 TargetToSpawn = GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
	if (!ShouldKeepSpawnEnemies() && CurrentSpawnedEnemiesCounter == 0 && TotalSpawnedEnemiesThisWaveCounter >= TargetToSpawn)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
		SetCurrentBoss06GameMode(EBoss06GameModeState::WaveComplete);
	}
}

void ABoss06GameMode::OnEnemyDied(ABaseEnemyCharacter* Enemy)
{
	CurrentSpawnedEnemiesCounter--;
	
	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	const int32 TargetToSpawn = GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
	if (!ShouldKeepSpawnEnemies() && CurrentSpawnedEnemiesCounter == 0 && TotalSpawnedEnemiesThisWaveCounter >= TargetToSpawn)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
		SetCurrentBoss06GameMode(EBoss06GameModeState::WaveComplete);
	}
}

void ABoss06GameMode::RegisterSpawnedEnemies(const TArray<ABaseEnemyCharacter*>& InEnemiesToRegister)
{
	for (ABaseEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
		
			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
			
			CurrentSpawnedEnemiesCounter++;
			TotalSpawnedEnemiesThisWaveCounter++;
			
		}
	}
}