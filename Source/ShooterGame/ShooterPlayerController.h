// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShooterPlayerController();

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;


protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> CurrentLevelInformationClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* CurrentLevelInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LevelCompleteClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* LevelComplete;

	FTimerHandle RestartTimer;

public:

	void ShowCurrentLevel();
	void ShowLevelComplete();
};
