// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Enemy.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
    NodeName = "Chect Attack Range";
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn == nullptr)
    {
        return;
        UE_LOG(LogTemp, Warning, TEXT("NULL"));
    }

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    auto EnemyCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if (EnemyCharacter)
    {
        const float RangeToEnemy = (PlayerPawn->GetActorLocation() - EnemyCharacter->GetActorLocation()).Size();

        UE_LOG(LogTemp, Warning, TEXT("%f"), RangeToEnemy);

        if (EnemyCharacter->GetAttackRange() >= RangeToEnemy)
        {
            
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
        }
        else
        {
            
            OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
        

}
