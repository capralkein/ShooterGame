// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"
#include "Weapon.h"
#include "Components/SceneComponent.h"
#include "Engine/SkeletalMeshSocket.h"


AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false),
	Ammo(30),
	MagazineCapacity(30),
	WeaponType(EWeaponType::EWT_SubmachineGun),
	AmmoType(EAmmoType::EAT_9mm),
	ReloadMontageSection(FName(TEXT("Reload SMG"))),
	ClipBoneName(TEXT("smg_clip")),
	SlideDisplacement(0.f),
	SlideDisplacementTime(0.2f),
	bMovingSlide(false),
	MaxSlideDisplacement(4.f),
	MaxRecoilRotation(20.f),
	bAutomatic(true)

{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep the Weapon upright
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	UpdateSlideDisplacement();
}

void AWeapon::PickupUpgrade(AUpgrade* Upgrade)
{	
	if (UpgradeSlots.Num() < UPGRADESLOTS_CAPACITY)
	{
		Upgrade->SetSlotIndex(UpgradeSlots.Num());
		UpgradeSlots.Add(Upgrade);
		Upgrade->SetItemState(EItemState::EIS_PickedUp);
		Upgrade->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("UpgradeSocket"));
		Upgrade->ApplyEffect(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TODO Swap upgrades"));
	}
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
	const FVector MeshRight{ GetItemMesh()->GetRightVector() };
	// Direction in which we throw the Weapon
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	float RandomRotation{ FMath::FRandRange(2, 10) };
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= 5'000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

	EnableGlowMaterial();
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
	StartPulseTimer();
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FString WeaponTablePath{ TEXT("DataTable'/Game/_Game/DataTables/WeaponDataTable.WeaponDataTable'") };
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponDataRow = nullptr;

		if(!GetIsDeliberate())
		{
			WeaponType = static_cast<EWeaponType>(FMath::RandRange(0, 2));
		}

		switch (WeaponType)
		{
		case EWeaponType::EWT_SubmachineGun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SubmachineGun"), TEXT(""));
			break;
		case EWeaponType::EWT_AssaultRifle:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
			break;
		case EWeaponType::EWT_Pistol:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
			break;
		}

		if (WeaponDataRow)
		{
			RarityMultiplier = WeaponDataRow->RarityMultiplier;

			AmmoType = WeaponDataRow->AmmoType;
			ProjectileClass = WeaponDataRow->ProjectileClass;

			MagazineCapacity = WeaponDataRow->MagazineCapacity * pow(RarityMultiplier, GetNumberOfStars() - 1);
			Ammo = MagazineCapacity;

			SetPickupSound(WeaponDataRow->PickupSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetItemName(WeaponDataRow->ItemName);
			SetIconItem(WeaponDataRow->InventoryIcon);
			SetAmmoIcom(WeaponDataRow->AmmoIcon);

			SetMaterialInstance(WeaponDataRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponDataRow->MaterialIndex);
			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);

			CrosshairsMiddle = WeaponDataRow->CrosshairsMiddle;
			CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
			CrosshairsRight = WeaponDataRow->CrosshairsRight;
			CrosshairsBottom = WeaponDataRow->CrosshairsBottom;
			CrosshairsTop = WeaponDataRow->CrosshairsTop;

			HeadShotDamage = WeaponDataRow->HeadShotDamage; // mb delete

			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			BoneToHide = WeaponDataRow->BoneToHide;
			bAutomatic = WeaponDataRow->bAutomatic;

			BaseDamage = WeaponDataRow->Damage * pow(RarityMultiplier, GetNumberOfStars() - 1);
			Damage = BaseDamage;

			DamageArea = WeaponDataRow->DamageArea;
			ProjectileSpeed = WeaponDataRow->ProjectileSpeed;

			CritMultiplier = WeaponDataRow->CritMultiplier * pow(RarityMultiplier, GetNumberOfStars() - 1);

			BaseAutoFireRate = WeaponDataRow->AutoFireRate * pow(RarityMultiplier, GetNumberOfStars() - 1);
			AutoFireRate = BaseAutoFireRate;
		}

		

		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("FresnelColor"), GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());
			EnableGlowMaterial();
		}

		SetIsDeliberate(true);
	}
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (BoneToHide != FName(""))
	{
		GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
	}
}

void AWeapon::FinishMovingSlide()
{
	bMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if (SlideDisplacementCurve && bMovingSlide)
	{
		const float ElapsedTime{ GetWorldTimerManager().GetTimerElapsed(SlideTimer) };
		const float CurveValue{ SlideDisplacementCurve->GetFloatValue(ElapsedTime) };
		SlideDisplacement = CurveValue * MaxSlideDisplacement;
		RecoilRotation = CurveValue * MaxRecoilRotation;
	}

}

void AWeapon::DecrementAmmo()
{
	if (Ammo - 1 <= 0)
	{
		Ammo = 1;
	}
	--Ammo;
}

void AWeapon::StartSlideTimer()
{
	bMovingSlide = true;
	GetWorldTimerManager().SetTimer(SlideTimer, this, &AWeapon::FinishMovingSlide, SlideDisplacementTime);
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("reload with more magazine capacity"));
	Ammo += Amount;
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MagazineCapacity;
}
