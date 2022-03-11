// Fill out your copyright notice in the Description page of Project Settings.

#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "Enemy.h"
#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Use a sphere as a simple collision representation.
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Set the sphere's collision radius.
	CollisionSphere->InitSphereRadius(15.0f);
	CollisionSphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	SetRootComponent(CollisionSphere);
	
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(GetRootComponent());

	ExplodeAreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplodeAreaSphere"));
	ExplodeAreaSphere->SetupAttachment(GetRootComponent());
	ExplodeAreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ExplodeAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionSphere);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	// Delete the projectile after 3 seconds.
	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	if (TrailParticles)
	{
		UGameplayStatics::SpawnEmitterAttached
		(
			TrailParticles,
			ProjectileMeshComponent,
			FName("None"),
			EmitterSpawnLocation
		);
	}

	if (LifeSound)
	{
		UGameplayStatics::SpawnSoundAttached(
			LifeSound,
			GetRootComponent()			
		);
	}

	
	
}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			ExplodeParticles,
			GetActorLocation(),
			FRotator(0.f),
			FVector(EmitterSize),
			true
		);
	}

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	AShooterCharacter* Shooter = Cast<AShooterCharacter>(GetOwner());
	AEnemy* EnemyShooter = Cast<AEnemy>(GetOwner());
	if (Shooter)
	{
		for (auto Actor : OverlappingActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor damaged by explosive: %s"), *Actor->GetName());
			AEnemy* HitEnemy = Cast<AEnemy>(Actor);
			if (HitEnemy)
			{
				UGameplayStatics::ApplyDamage
				(
					HitEnemy,
					Shooter->GetEquippedWeaponDamage(),
					Shooter->GetController(),
					Shooter,
					UDamageType::StaticClass()
				);
				HitEnemy->ShowHitNumber(Shooter->GetEquippedWeaponDamage(), Actor->GetActorLocation(), false);
			}
			else
			{
				if (Actor)
				{
					UGameplayStatics::ApplyDamage
					(
						Actor,
						Shooter->GetEquippedWeaponDamage(),
						Shooter->GetController(),
						Shooter,
						UDamageType::StaticClass()
					);
				}
			}
			
		}
	}
	if (EnemyShooter)
	{
		
		AShooterCharacter* HitShooter = Cast<AShooterCharacter>(OtherActor);
		if (HitShooter)
		{
			UGameplayStatics::ApplyDamage
			(
				HitShooter,
				EnemyShooter->GetDamage(),
				EnemyShooter->GetController(),
				EnemyShooter,
				UDamageType::StaticClass()
			);
			

			
			if (HitShooter->GetMeleeImpactSound())
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitShooter->GetMeleeImpactSound(),
					GetActorLocation()
				);
			}

		}
		
	}
	Destroy();
	
}

// Called every frame

void ABullet::SetBulletProperties(float Speed, float DamageArea)
{
	if (Speed == 0) Speed = 3000.f;

	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;

	if (DamageArea == 0)
	{
		DamageArea = 50;
		ExplodeAreaSphere->SetSphereRadius(DamageArea);
	}
	else
	{
		ExplodeAreaSphere->SetSphereRadius(DamageArea);
		EmitterSize = ExplodeAreaSphere->GetScaledSphereRadius() / 300;
	}
	ProjectileMeshComponent->SetWorldScale3D(FVector(EmitterSize));
}

