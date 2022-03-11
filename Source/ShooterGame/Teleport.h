// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleport.generated.h"

UCLASS()
class SHOOTERGAME_API ATeleport : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTeleportOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
	class UParticleSystem* TeleportParticles;

	class UParticleSystemComponent* SpawnedTeleportParticles;

	FTimerHandle TeleportDelayTimer;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* TeleportCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateTeleport();

	void DeactivateTeleport();

};
