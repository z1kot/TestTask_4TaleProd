#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	Health = 1.0f;
	bDead = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		MaxHealth = GetHealth();
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::ReceiveOnAnyDamage);
	}
}

void UHealthComponent::ReceiveOnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetDead())
	{
		return;
	}

	SetHealth(Health - Damage);

	if (GetHealth() == 0.0f)
	{
		SetDead(true);
	}
}

void UHealthComponent::SetHealth(float Value)
{ 
	Health = FMath::Clamp(Value, 0.0f, GetMaxHealth());
	OnRep_SetHealth();
}

void UHealthComponent::SetDead(bool bValue)
{
	bDead = bValue;
	OnRep_SetDead();
}

void UHealthComponent::OnRep_SetHealth()
{
	HealthChangedDelegate.Broadcast(Health, MaxHealth);
}

void UHealthComponent::OnRep_SetDead()
{
	DeadDelegate.Broadcast(bDead);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, bDead);
}