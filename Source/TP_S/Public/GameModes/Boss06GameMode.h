// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BaseEnemyCharacter.h"
#include "GameModes/BaseGameMode.h"
#include "Boss06GameMode.generated.h"

class ABaseEnemyCharacter;

UENUM(BlueprintType)
enum class EBoss06GameModeState : uint8
{
	WaitSpawnNewWave,
	SpawnNewWave,
	Inprogress,
	WaveComplete,
	AllWavesDone,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ABaseEnemyCharacter>SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 5;
};

USTRUCT(BlueprintType)
struct FBaseEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FEnemyWaveSpawnerInfo>EnemyWaveSpawnerDefinition;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnABoss06GameModeStateChangedDelegate, EBoss06GameModeState, CurrentState); 
/**
 * 
 */
UCLASS()
class TP_S_API ABoss06GameMode : public ABaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	void SetCurrentBoss06GameMode(EBoss06GameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	FBaseEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	UPROPERTY()
	EBoss06GameModeState CurrentBoss06GameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnABoss06GameModeStateChangedDelegate OnBoss06GameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*> TargetPointsArray;
	
	UPROPERTY()
	float TimePassedSinceStart = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap<TSoftClassPtr<ABaseEnemyCharacter>,UClass*>PreLoadedEnemyClassMap;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<ABaseEnemyCharacter*>& InEnemiesToRegister);
};
	

