// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"


AShooterPlayerController::AShooterPlayerController()
{

}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUDOverlay->RemoveFromViewport();

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, 0.8f);
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}

	
}

void AShooterPlayerController::ShowCurrentLevel()
{
	if (CurrentLevelInformationClass)
	{
		CurrentLevelInformation = CreateWidget<UUserWidget>(this, CurrentLevelInformationClass);
		if (CurrentLevelInformation)
		{
			CurrentLevelInformation->AddToViewport();
			CurrentLevelInformation->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AShooterPlayerController::ShowLevelComplete()
{
	if (LevelCompleteClass)
	{
		LevelComplete = CreateWidget<UUserWidget>(this, LevelCompleteClass);
		if (LevelComplete)
		{
			LevelComplete->AddToViewport();
			LevelComplete->SetVisibility(ESlateVisibility::Visible);
		}
	}
}