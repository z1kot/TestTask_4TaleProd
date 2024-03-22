#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/PlayerWeaponInterface.h"
#include "BaseWeaponProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	bReplicates = true;
	AttachSocketName = FName(TEXT("WeaponSocket"));

	Projectile = ABaseWeaponProjectile::StaticClass();

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(SceneComponent);

	ReloadAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadAudio"));
	ReloadAudio->SetupAttachment(SceneComponent);

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(SceneComponent);

	MuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleParticle"));
	MuzzleParticle->SetupAttachment(MuzzleComponent);
}

void ABaseWeapon::InitParams()
{
	WeaponData = Cast<UWeaponData>(ItemData);

	CurrentAmmo = WeaponData->Ammo;
	Damage = WeaponData->Damage;
	UseRate = WeaponData->UseRate;
	bAutoFire = WeaponData->bAutoFire;
	ReloadRate = WeaponData->ReloadRate;
	bInfinityAmmo = WeaponData->bInfinityAmmo;
}

void ABaseWeapon::UseItem()
{
	if (HasAuthority())
	{
		UseWeapon();
	}
}

void ABaseWeapon::StopUseItem()
{
	bFireActive = false;
}

bool ABaseWeapon::AbleToUse()
{
	return !bUseFreezedByRate && (bInfinityAmmo || CurrentAmmo > 0);
}

void ABaseWeapon::UseWeapon_Implementation()
{
	if (AbleToUse())
	{
		if (!bInfinityAmmo)
		{
			CurrentAmmo--;
		}

		bUseFreezedByRate = true;
		bFireActive = true;

		GetWorld()->GetTimerManager().SetTimer(UseRateTimer, this, &ABaseWeapon::StopRateDelay, UseRate);

		Fire();

		// Cosmetic function
		MulticastWeaponFired();

		return;
	}

	if (CurrentAmmo == 0 && !bUseFreezedByRate)
	{
		TryReloadWeapon();

		bFireActive = false;
		return;
	}

	bFireActive = false;
	return;
}

void ABaseWeapon::Fire()
{
	if (Projectile && GetOwner())
	{
		if (GetOwner()->Implements<UPlayerWeaponInterface>())
		{
			FRotator FireDir = IPlayerWeaponInterface::Execute_GetFireDir(GetOwner()).ToOrientationRotator();

			// Get Projectile start location
			FVector SpawnLocation = MuzzleComponent->GetComponentLocation();

			// Shoot Projectile
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ABaseWeaponProjectile* ActorProjectile = Cast<ABaseWeaponProjectile>(GetWorld()->SpawnActor(Projectile, &SpawnLocation, &FireDir, SpawnParams));

			ActorProjectile->InitParams(Damage);

			return;
		}
	}
}

void ABaseWeapon::StopRateDelay()
{
	bUseFreezedByRate = false;
	GetWorld()->GetTimerManager().ClearTimer(UseRateTimer);

	// Cosmetic function
	MulticastWeaponReadyToFire();

	// Auto fire logic
	if (bAutoFire && bFireActive)
	{
		UseWeapon();
	}
}

void ABaseWeapon::TryReloadWeapon()
{
	GetWorld()->GetTimerManager().ClearTimer(UseRateTimer);

	bUseFreezedByRate = true;
	GetWorld()->GetTimerManager().SetTimer(UseRateTimer, this, &ABaseWeapon::ReloadAmmo, ReloadRate);

	// Cosmetic function
	MulticastStartReloading();
}

void ABaseWeapon::ReloadAmmo()
{
	CurrentAmmo = WeaponData->Ammo;
	bUseFreezedByRate = false;

	// Cosmetic function
	MulticastWeaponReadyToFire();
}

void ABaseWeapon::MulticastWeaponFired_Implementation()
{
	OnWeaponFired();
}

void ABaseWeapon::MulticastStartReloading_Implementation()
{
	OnStartReloading();
}

void ABaseWeapon::MulticastWeaponReadyToFire_Implementation()
{
	OnWeaponReadyToFire();
}

void ABaseWeapon::HideAndAttach()
{
	Super::HideAndAttach();

	// Hide Ammo Counter(value -1) and clear all binds
	AmmoChangedDelegate.Broadcast(-1);
	AmmoChangedDelegate.Clear();
}

const FVector ABaseWeapon::GetMuzzleLocation()
{
	return MuzzleComponent->GetComponentLocation();
}

void ABaseWeapon::OnRep_SetCurrentAmmo()
{
	AmmoChangedDelegate.Broadcast(CurrentAmmo);
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, WeaponData);
	DOREPLIFETIME(ABaseWeapon, CurrentAmmo);
}