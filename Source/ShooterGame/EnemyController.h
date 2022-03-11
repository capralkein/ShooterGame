// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = true))
	class UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

public:

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackBoardComponent; }
};
