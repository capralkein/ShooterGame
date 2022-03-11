// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

AEnemyController::AEnemyController()
{
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	check(BlackBoardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr) return;

	AEnemy* Enemy = Cast<AEnemy>(InPawn);
	if (Enemy)
	{
		if (Enemy->GetBehaviorTree())
		{
			BlackBoardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
			
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			BlackBoardComponent->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		}
	}
}