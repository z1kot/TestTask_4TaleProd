#include "BaseWeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


ABaseWeaponProjectile::ABaseWeaponProjectile()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 2.0f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(10.0f);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(SphereCollision);

	FlyAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("FlyAudio"));
	FlyAudio->SetupAttachment(SphereCollision);

	TracerParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TracerParticle"));
	TracerParticle->SetupAttachment(SphereCollision);
}

void ABaseWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeaponProjectile::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, GetOwner(), nullptr);

		CosmeticHit(SweepResult);

		Destroy();
	}
}

void ABaseWeaponProjectile::InitParams(const float NewDamage)
{
	Damage = NewDamage;
}

void ABaseWeaponProjectile::CosmeticHit_Implementation(const FHitResult& SweepResult)
{
	// Due to projectile destroying we need to spawn hit effects
	UGameplayStatics::SpawnSoundAtLocation(this, HitSound, SweepResult.ImpactPoint);

	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, SweepResult.ImpactPoint, SweepResult.ImpactNormal.ToOrientationRotator());
}