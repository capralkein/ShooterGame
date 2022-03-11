// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShooterCharacter.h"
#include "ShooterGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Teleport.h"

// Sets default values
ATeleport::ATeleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeleportCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TeleportCollision"));
	TeleportCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeleport::BeginPlay()
{
	Super::BeginPlay();

	TeleportCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeleport::OnTeleportOverlap);

	//TeleportCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TeleportCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TeleportCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	TeleportCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TeleportCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);	

	
}

void ATeleport::OnTeleportOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AShooterCharacter>(OtherActor);

	auto GameMode = GetWorld()->GetAuthGameMode<AShooterGameGameModeBase>();

	if (GameMode)
	{
		if (Character)
		{
			GameMode->CharacterCanTeleport(Character);

		}
	}

	
}


// Called every frame
void ATeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeleport::ActivateTeleport()
{
	if (TeleportParticles)
	{
		SpawnedTeleportParticles = UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			TeleportParticles,
			GetActorLocation(),
			FRotator(-90.f, 0.f, 0.f),
			FVector(4.f),
			true
		);
	}
}

void ATeleport::DeactivateTeleport()
{
	if (SpawnedTeleportParticles)
	{
		SpawnedTeleportParticles->DestroyComponent();
	}
}

