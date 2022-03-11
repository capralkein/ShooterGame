// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxGameMode.h"
#include "ShooterCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "EnemyController.h"
#include "Teleport.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"


void ASandboxGameMode::CharacterKilled(APawn* PawnKilled)
{
	Super::CharacterKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->GameHasEnded(PlayerController->GetPawn(), false);
	}
}

/*void ASandboxGameMode::EndGame(bool bIsPlayerWinner)
{
	
}*/

void ASandboxGameMode::EnemyKilled(APawn* PawnKilled)
{
	Super::EnemyKilled(PawnKilled);

	EnemyAliveCount--;

	SpawnLootEvent(PawnKilled);

	UE_LOG(LogTemp, Warning, TEXT("Enemy killed! %i left"), EnemyAliveCount);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (EnemyAliveCount <= 0 && EnemiesShouldSpawn == 0)
	{
		CurrentLevel++;

		LevelFinished();
		LevelToLoadName = FName(TEXT("None"));
		bLevelFinished = true;
		EnableTeleport();

		AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetController());
		if (ShooterController)
		{
			ShooterController->ShowLevelComplete();
		}
	}
}

void ASandboxGameMode::EnemySpawned(APawn* PawnSpawned)
{
	Super::EnemySpawned(PawnSpawned);

	EnemyAliveCount++;
	EnemiesShouldSpawn--;
}

bool
ASandboxGameMode::CharacterCanTeleport(AShooterCharacter* Character)
{
	if (bCharacterCanTeleport)
	{
		bCharacterCanTeleport = false;

		Character->PlayRecallEffect(); // effect

		FLatentActionInfo LatentInfo;

		if (!bLevelFinished)
		{
			SetUpRules();
			UGameplayStatics::LoadStreamLevel(this, LevelToLoadName, true, true, LatentInfo);
		}
		
		GetWorldTimerManager().SetTimer(
			TeleportDelayTimer,
			this,
			&ASandboxGameMode::Teleport,
			0.5f);

		return true;
	}

	return false;
}

void ASandboxGameMode::EnableTeleport()
{
	bCharacterCanTeleport = true;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeleport::StaticClass(), FoundActors);
	
	for (auto Actor : FoundActors)
	{
		ATeleport* ActorToActivate = Cast<ATeleport>(Actor);
		if (ActorToActivate)
		{
			ActorToActivate->ActivateTeleport();
		}
	}

}

void ASandboxGameMode::DisableTeleport()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeleport::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ATeleport* ActorToDeactivate = Cast<ATeleport>(Actor);
		if (ActorToDeactivate)
		{
			ActorToDeactivate->DeactivateTeleport();
		}
	}
}

void ASandboxGameMode::Teleport()
{
	auto Character = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	DisableTeleport();

	const FString LevelsTablePath{ TEXT("DataTable'/Game/_Game/DataTables/LevelsDataTable.LevelsDataTable'") };
	UDataTable* LevelsTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *LevelsTablePath));

	if (LevelsTableObject)
	{
		FLevelsDataTable* LevelsDataRow = nullptr;

		if (bLevelFinished)
		{
			LevelsDataRow = LevelsTableObject->FindRow<FLevelsDataTable>(FName("Base"), TEXT(""));
		}
		else
		{
			LevelsDataRow = LevelsTableObject->FindRow<FLevelsDataTable>(LevelToLoadName, TEXT(""));
		}

		if (LevelsDataRow && Character)
		{
			Character->SetActorLocation(LevelsDataRow->TeleportLocation);

			AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(Character->GetController());
			if (PlayerController)
			{
				PlayerController->SetControlRotation(LevelsDataRow->TeleportRotation);
			}

			Character->PlayTeleportEffect();			

			if (!bLevelFinished)
			{
				FLatentActionInfo LatentInfo;
				CurrentLevelFogName = LevelsDataRow->LevelFogName;
				UGameplayStatics::LoadStreamLevel(this, CurrentLevelFogName, true, false, LatentInfo);

				CurrentLevelName = LevelToLoadName;

				if (PlayerController)
				{
					LevelDisplayName = LevelsDataRow->LevelDisplayName;
					PlayerController->ShowCurrentLevel();
				}
			}
			else
			{
				FLatentActionInfo LatentInfo;
				UGameplayStatics::UnloadStreamLevel(this, CurrentLevelFogName, LatentInfo, false);

				DisableTeleport();

				GetWorldTimerManager().SetTimer(
					TeleportDelayTimer,
					this,
					&ASandboxGameMode::UnloadLevel,
					0.5f);
				bLevelFinished = false;

				bCharacterCanTeleport = false;
			}
		}
	}
}

void ASandboxGameMode::UnloadLevel()
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, CurrentLevelName, LatentInfo, false);	
}

void ASandboxGameMode::BeginPlay()
{
	Super::BeginPlay();
	bCharacterCanTeleport = false;
	bLevelFinished = false;
}

void ASandboxGameMode::SetUpRules()
{
	EnemiesShouldSpawn = CurrentLevel * Difficulty;
	LevelToLoadName = LevelsNames[FMath::RandRange(0, 1)];

	UE_LOG(LogTemp, Warning, TEXT("CurrentLevel %i"), CurrentLevel);
	UE_LOG(LogTemp, Warning, TEXT("EnemiesShouldSpawn %i"), EnemiesShouldSpawn);
}
