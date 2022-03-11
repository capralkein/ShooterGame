// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void CharacterKilled(APawn* PawnKilled);

	virtual void EnemyKilled(APawn* PawnKilled);

	virtual void EnemySpawned(APawn* PawnSpawned);

	virtual bool CharacterCanTeleport(class AShooterCharacter* Character);

	virtual void EnableTeleport();

	virtual void DisableTeleport();
};
