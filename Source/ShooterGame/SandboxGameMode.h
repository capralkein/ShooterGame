// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShooterGameGameModeBase.h"
#include "SandboxGameMode.generated.h"

USTRUCT(BlueprintType)
struct FLevelsDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelFogName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TeleportLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TeleportRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelDisplayName;
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ASandboxGameMode : public AShooterGameGameModeBase
{
	GENERATED_BODY()

public:
	virtual void CharacterKilled(APawn* PawnKilled) override;

	virtual void EnemyKilled(APawn* PawnKilled) override;

	virtual void EnemySpawned(APawn* PawnSpawned) override;

	virtual bool CharacterCanTeleport(AShooterCharacter* Character) override;

	virtual void EnableTeleport() override;

	virtual void DisableTeleport() override;

	void Teleport();

	void UnloadLevel();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLootEvent(APawn* PawnKilled);

	UFUNCTION(BlueprintImplementableEvent)
	void LevelFinished();

	void SetUpRules();

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	int32 Difficulty = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	int32 EnemiesShouldSpawn = CurrentLevel * Difficulty;

	//void EndGame(bool bIsPlayerWinner);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	int32 EnemyAliveCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	FName CurrentLevelName;

	UPROPERTY(EditAnywhere)
	FName CurrentLevelFogName;

	UPROPERTY(EditAnywhere)
	FName BaseLevelName;

	UPROPERTY(EditAnywhere)
	FName LevelToLoadName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Properties", meta = (AllowPrivateAccess = "true"))
	FName LevelDisplayName;

	UPROPERTY(EditAnywhere)
	TArray<FName> LevelsNames;

	bool bLevelFinished;

	bool bCharacterCanTeleport;

	FTimerHandle TeleportDelayTimer;


public:
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE int32 GetDifficulty() const { return Difficulty; }
	FORCEINLINE int32 GetEnemiesShouldSpawn() const { return EnemiesShouldSpawn; }
	FORCEINLINE int32 GetEnemyAliveCount() const { return EnemyAliveCount; }
};

