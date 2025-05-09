// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "WarriorSurvivalGameMode.generated.h"


class AWarriorEnemyCharacter;

UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};
 
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn;
 
	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;
 
	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};
 
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;
 
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate,EWarriorSurvivalGameModeState,CurrentState);

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorSurvivalGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister);
	
protected:
 	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
 
private:
	void SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	FWarriorEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;
	
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
 
	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;
 
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSurvialGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;
 
	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;
 
	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;
 
	UPROPERTY()
	TArray<AActor*> TargetPointsArray;
 
	UPROPERTY()
	float TimePassedSinceStart = 0.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;
	
	UPROPERTY()
	TMap< TSoftClassPtr < AWarriorEnemyCharacter >,UClass* > PreLoadedEnemyClassMap;
	
};
