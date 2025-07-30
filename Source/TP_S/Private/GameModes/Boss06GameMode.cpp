// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Boss06GameMode.h"
#include "Engine/AssetManager.h"
#include "Character/Enemy/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include  "DebugHelper.h"
#include "EngineUtils.h"

void ABoss06GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	
	if (!BossSpawnPoint)
	{
		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(FName("BossSpawn")))
			{
				BossSpawnPoint = *It;
				UE_LOG(LogTemp, Warning, TEXT("✅ BossSpawnPoint 자동 할당 (태그 기반): %s"), *BossSpawnPoint->GetName());
				break;
			}
		}
		if (!BossSpawnPoint)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ BossSpawnPoint is not assigned"));
		}
	}
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
			TimePassedSinceStart = 0.f;

			const int32 SpawnedThisFrame = TrySpawnWaveEnemies();
			CurrentSpawnedEnemiesCounter += SpawnedThisFrame;
			
			if (!ShouldKeepSpawnEnemies())
			{
				SetCurrentBoss06GameMode(EBoss06GameModeState::Inprogress);
			}
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
	if (TargetPointsArray.IsEmpty() || !IsValid(TargetPointsArray[0]))
	{
		TargetPointsArray.Empty();
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}
	checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid Target point found in level: %s for spawning enemies"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FBaseEnemyWaveSpawnerTableRow* WaveRow = GetCurrentWaveSpawnerTableRow();
	if (!WaveRow) return 0;

	for (const FEnemyWaveSpawnerInfo& SpawnerInfo : WaveRow->EnemyWaveSpawnerDefinition)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		UClass** FoundClass = PreLoadedEnemyClassMap.Find(SpawnerInfo.SoftEnemyClassToSpawn);
		if (!FoundClass) continue;

		UClass* LoadedEnemyClass = *FoundClass;
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			FVector SpawnLocation;
			FRotator SpawnRotation;

			if (SpawnerInfo.bIsBoss)
			{
				// ✅ 보스는 고정 위치에서만 스폰
				if (!IsValid(BossSpawnPoint))
				{
					UE_LOG(LogTemp, Error, TEXT("BossSpawnPoint is not assigned!"));
					continue;
				}
				SpawnLocation = BossSpawnPoint->GetActorLocation() + FVector(0.f, 0.f, 150.f);
				SpawnRotation = BossSpawnPoint->GetActorRotation();
			}
			else
			{
				const int32 RandomTargetIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
				const FVector Origin = TargetPointsArray[RandomTargetIndex]->GetActorLocation();

				UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, Origin, SpawnLocation, 400.f);
				SpawnLocation += FVector(0.f, 0.f, 150.f);
				SpawnRotation = TargetPointsArray[RandomTargetIndex]->GetActorForwardVector().ToOrientationRotator();
			}

			ABaseEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemyCharacter>(
				LoadedEnemyClass, SpawnLocation, SpawnRotation, SpawnParam);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}
		}
	}
	return EnemiesSpawnedThisTime;
}

bool ABoss06GameMode::ShouldKeepSpawnEnemies() const
{
	if (CurrentBoss06GameModeState != EBoss06GameModeState::Inprogress)
		return false;

	FBaseEnemyWaveSpawnerTableRow* WaveRow = GetCurrentWaveSpawnerTableRow();
	if (!WaveRow) return false;
	return TotalSpawnedEnemiesThisWaveCounter < WaveRow->TotalEnemyToSpawnThisWave;
}

void ABoss06GameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{

	if (!IsValid(DestroyedActor))
		return;

	if (!EnemyWaveSpawnerDataTable || HasFinishedAllWaves())
		return;

	FBaseEnemyWaveSpawnerTableRow* CurrentWaveRow = GetCurrentWaveSpawnerTableRow();
	if (!CurrentWaveRow) return;
	
	if (!IsValid(this) || !EnemyWaveSpawnerDataTable || HasFinishedAllWaves())
		return;

	if (CurrentBoss06GameModeState != EBoss06GameModeState::Inprogress)
		return;
	
	const int32 TargetToSpawn = CurrentWaveRow->TotalEnemyToSpawnThisWave;
	const bool bNeedMoreEnemies = TotalSpawnedEnemiesThisWaveCounter < TargetToSpawn;
	
	CurrentSpawnedEnemiesCounter--;

	if (bNeedMoreEnemies)
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	if (!bNeedMoreEnemies && CurrentSpawnedEnemiesCounter <= 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
		SetCurrentBoss06GameMode(EBoss06GameModeState::WaveComplete);
	}
	
}

void ABoss06GameMode::OnEnemyDied(ABaseEnemyCharacter* Enemy)
{
	if (!IsValid(this) || !EnemyWaveSpawnerDataTable || HasFinishedAllWaves())
		return;

	if (CurrentBoss06GameModeState != EBoss06GameModeState::Inprogress)
		return;

	FBaseEnemyWaveSpawnerTableRow* CurrentWaveRow = GetCurrentWaveSpawnerTableRow();
	if (!CurrentWaveRow) return;

	const int32 TargetToSpawn = CurrentWaveRow->TotalEnemyToSpawnThisWave;
	const bool bNeedMoreEnemies = TotalSpawnedEnemiesThisWaveCounter < TargetToSpawn;

	CurrentSpawnedEnemiesCounter--;

	if (bNeedMoreEnemies)
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}

	const bool bWaveFinished = !bNeedMoreEnemies &&
		CurrentSpawnedEnemiesCounter <= 0 &&
		TotalSpawnedEnemiesThisWaveCounter >= TargetToSpawn;

	if (bWaveFinished)
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

void ABoss06GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!GetWorld()) return;
	
	for (TActorIterator<ABaseEnemyCharacter> It(GetWorld()); It; ++It)
	{
		ABaseEnemyCharacter* Enemy = *It;
		if (Enemy && !Enemy->HasAnyFlags(RF_BeginDestroyed))
		{
			Enemy->OnDestroyed.RemoveDynamic(this, &ABoss06GameMode::OnEnemyDestroyed);
		}
	}
}

void ABoss06GameMode::CleanupBeforeMapChange()
{
	if (!GetWorld()) return;

	for (TActorIterator<ABaseEnemyCharacter> It(GetWorld()); It; ++It)
	{
		ABaseEnemyCharacter* Enemy = *It;
		if (Enemy && !Enemy->HasAnyFlags(RF_BeginDestroyed))
		{
			// ✅ Delegate 안전 해제
			Enemy->OnDestroyed.RemoveDynamic(this, &ABoss06GameMode::OnEnemyDestroyed);
		}
	}
}