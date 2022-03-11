// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ShooterGame/AmmoType.h"
#include "Upgrade.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PercentageDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChangeAmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageArea;

};

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	EUT_AddDamage UMETA(DisplayName = "AddDamage"),
	EUT_AddFireRate UMETA(DisplayName = "AddFireRate"),
	EUT_Explosive UMETA(DisplayName = "Explosive"),

	EUT_MAX UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AUpgrade : public AItem
{
	GENERATED_BODY()

public:
	AUpgrade();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void SetItemProperties(EItemState State) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* UpgradeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EUpgradeType UpgradeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float PercentageDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float DamageArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bChangeAmmoType;

public:

	FORCEINLINE UStaticMeshComponent* GetUpgradeMesh() const { return UpgradeMesh; }

	void ApplyEffect(class AWeapon* WeaponToUpgrade);

	virtual void EnableCustomDepth() override;
	virtual void DisableCustomDepth() override;
};
