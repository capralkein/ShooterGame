// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"

AUpgrade::AUpgrade()
{
	UpgradeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpgradeMesh"));
	SetRootComponent(UpgradeMesh);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());
}

void AUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpgradeMesh->AddLocalRotation(FRotator(0.f, 1.f, 0.f));
}

void AUpgrade::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);


	const FString UpgradeTablePath{ TEXT("DataTable'/Game/_Game/DataTables/UpgradeDataTable.UpgradeDataTable'") };
	UDataTable* UpgradeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *UpgradeTablePath));

	if (UpgradeTableObject)
	{
		FUpgradeDataTable* UpgradeDataRow = nullptr;

		
		UpgradeType = static_cast<EUpgradeType>(FMath::RandRange(0, 2));
		

		switch (UpgradeType)
		{
		case EUpgradeType::EUT_AddDamage :
			UpgradeDataRow = UpgradeTableObject->FindRow<FUpgradeDataTable>(FName("Add50Damage"), TEXT(""));
			break;
		case EUpgradeType::EUT_AddFireRate:
			UpgradeDataRow = UpgradeTableObject->FindRow<FUpgradeDataTable>(FName("AddFireRate"), TEXT(""));
			break;
		case EUpgradeType::EUT_Explosive:
			UpgradeDataRow = UpgradeTableObject->FindRow<FUpgradeDataTable>(FName("Explosive"), TEXT(""));
			break;
		}

		if (UpgradeDataRow)
		{
			SetItemName(UpgradeDataRow->ItemName);
			SetItemDescription(UpgradeDataRow->ItemDescription);
			SetIconItem(UpgradeDataRow->InventoryIcon);

			PercentageDamage = UpgradeDataRow->PercentageDamage;
			FireRate = UpgradeDataRow->FireRate;	
			bChangeAmmoType = UpgradeDataRow->bChangeAmmoType;
			AmmoType = UpgradeDataRow->AmmoType;
			DamageArea = UpgradeDataRow->DamageArea;
		}
	}
}

void AUpgrade::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgrade::SetItemProperties(EItemState State)
{
	Super::SetItemProperties(State);

	switch (State)
	{
	case EItemState::EIS_Pickup:
		UpgradeMesh->SetSimulatePhysics(false);
		UpgradeMesh->SetEnableGravity(false);
		UpgradeMesh->SetVisibility(true);
		UpgradeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Equipped:
		UpgradeMesh->SetSimulatePhysics(false);
		UpgradeMesh->SetEnableGravity(false);
		UpgradeMesh->SetVisibility(true);
		UpgradeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:
		UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UpgradeMesh->SetSimulatePhysics(true);
		UpgradeMesh->SetEnableGravity(true);
		UpgradeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UpgradeMesh->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_WorldStatic,
			ECollisionResponse::ECR_Block);
		break;
	case EItemState::EIS_EquipInterping:
		UpgradeMesh->SetSimulatePhysics(false);
		UpgradeMesh->SetEnableGravity(false);
		UpgradeMesh->SetVisibility(true);
		UpgradeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_PickedUp:
		UpgradeMesh->SetSimulatePhysics(false);
		UpgradeMesh->SetEnableGravity(false);
		UpgradeMesh->SetVisibility(false);
		UpgradeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UpgradeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AUpgrade::ApplyEffect(AWeapon* WeaponToUpgrade)
{
	
	WeaponToUpgrade->SetDamage(WeaponToUpgrade->GetBaseDamage() * PercentageDamage);
	WeaponToUpgrade->SetAutoFireRate(WeaponToUpgrade->GetAutoFireRate() * FireRate);
	WeaponToUpgrade->SetDamageArea(WeaponToUpgrade->GetDamageArea() * DamageArea);

	if (bChangeAmmoType)
	{
		WeaponToUpgrade->SetAmmoType(AmmoType);
		UE_LOG(LogTemp, Warning, TEXT("Apply Effect"));
	}
}

void AUpgrade::EnableCustomDepth()
{
	UpgradeMesh->SetRenderCustomDepth(true);
}

void AUpgrade::DisableCustomDepth()
{
	UpgradeMesh->SetRenderCustomDepth(false);
}
