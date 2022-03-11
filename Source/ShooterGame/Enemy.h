// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulletHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTERGAME_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();


	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage(FName Section, float PlayRate);

	UFUNCTION(BlueprintPure)
	FName GetAttackSectionName();

	UFUNCTION()
	void OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ActivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void ActivateRightWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateRightWeapon();

	void DoDamage(class AShooterCharacter* Victim);
	void SpawnBlood(AShooterCharacter* Victim, FName SocketName);
	void StunCharacter(AShooterCharacter* Victim);

	void ResetCanAttack();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	UFUNCTION()
	void DestroyEnemy();

	

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	FString HeadBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	FString WeaponBone1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	FString WeaponBone2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitReactTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitReactTimeMax;

	bool bCanHitReact;

	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = true))
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitNumberDestroyTime;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	FVector PatrolPoint;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	FVector PatrolPoint2;

	class AEnemyController* EnemyController; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	bool bStunned;

	// 0-1 %
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	float StunChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	FName AttackLFast;
	FName AttackRFast;
	FName AttackL;
	FName AttackR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	class UBoxComponent* LeftWeaponCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName LeftWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName RightWeaponSocket;

	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	FTimerHandle AttackWaitTimer;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	bool bDying;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float DeathTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bHasRangedAttack;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABullet> ProjectileClass;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }
	FORCEINLINE FString GetWeaponBone1() const { return WeaponBone1; }
	FORCEINLINE FString GetWeaponBone2() const { return WeaponBone2; }

	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetDamage() const { return BaseDamage; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation, bool bHeadShot);

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }



};
