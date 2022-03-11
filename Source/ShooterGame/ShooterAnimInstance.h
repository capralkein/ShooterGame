// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"), 
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),

	EOS_MAX UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:

	void TurnInPlace();

	void Lean(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	// speed of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	// Character is in air?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInRoll;

	// Character moving?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	//offset yaw for strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	// Yaw of the Character this frame
	float TIPCharacterYaw;
	// Yaw of the Character last frame
	float TIPCharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	// rotation curve value this frame 
	float RotationCurve;
	// rotation curve value last frame
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;

	// Yaw of the Character this frame
	FRotator CharacterRotation;
	// Yaw of the Character last frame
	FRotator CharacterRotationLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lean", meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouching", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float RecoilWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bTurningInPlace;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouching", meta = (AllowPrivateAccess = "true"))
	bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EWeaponType EquippedWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bShouldUseFABRIK;


};
