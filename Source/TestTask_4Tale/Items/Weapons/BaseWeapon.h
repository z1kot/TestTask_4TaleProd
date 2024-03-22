#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "WeaponData.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChangedDelegate, int32, CurrentAmmo);

class ABaseWeaponProjectile;
class USceneComponent;
class UStaticMeshComponent;
class UAudioComponent;
class UParticleSystemComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Pistol = 1	UMETA(DisplayName = "Pistol"),
	Rifle = 2	UMETA(DisplayName = "Rifle"),
	RocketLaunhcer = 3	UMETA(DisplayName = "RocketLaunhcer")
};

UCLASS(Blueprintable)
class TESTTASK_4TALE_API ABaseWeapon : public ABaseItem
{
	GENERATED_BODY()

public:

	ABaseWeapon();

	/* PROPERTIES */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* ReloadAudio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseWeaponProjectile> Projectile;

	UPROPERTY(BlueprintReadWrite, Replicated)
	UWeaponData* WeaponData;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_SetCurrentAmmo)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadOnly)
	bool bAutoFire;

	UPROPERTY(BlueprintReadWrite)
	float UseRate;

	UPROPERTY(BlueprintReadWrite)
	float Damage;

	UPROPERTY(BlueprintReadWrite)
	float ReloadRate;

	UPROPERTY(BlueprintReadOnly)
	bool bInfinityAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* FireSound;

	UPROPERTY(BlueprintAssignable)
	FAmmoChangedDelegate AmmoChangedDelegate;

	UPROPERTY(BlueprintReadWrite)
	FName MuzzleSocketName = FName(TEXT("Muzzle"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::Pistol;

	/* FUNCTIONS */

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UseWeapon();

	virtual void UseItem() override;

	virtual void StopUseItem() override;

	virtual void InitParams() override;

	virtual void HideAndAttach() override;

protected:

	/* PROPERTIES */

	UPROPERTY(BlueprintReadWrite)
	bool bUseFreezedByRate;

	UPROPERTY(BlueprintReadWrite)
	bool bFireActive;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle UseRateTimer;

	/* FUNCTIONS */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool AbleToUse();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void StopRateDelay();

	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void ReloadAmmo();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastWeaponFired();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponFired();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastWeaponReadyToFire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponReadyToFire();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStartReloading();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartReloading();

	UFUNCTION(BlueprintPure)
	const FVector GetMuzzleLocation();

	UFUNCTION()
	void OnRep_SetCurrentAmmo();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
