#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UAudioComponent;
class UParticleSystemComponent;

UCLASS(Blueprintable)
class TESTTASK_4TALE_API ABaseWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseWeaponProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FlyAudio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* TracerParticle;

	UPROPERTY(BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* HitParticle;

public:

	UFUNCTION()
	void InitParams(const float NewDamage);

	UFUNCTION(BlueprintNativeEvent)
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void CosmeticHit(const FHitResult& SweepResult);

protected:

	virtual void BeginPlay() override;
};
