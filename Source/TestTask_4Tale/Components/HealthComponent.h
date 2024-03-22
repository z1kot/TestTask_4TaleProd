#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ScriptDelegates.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadDelegate, bool, bDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, float, NewHealth, float, MaxHealth);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASK_4TALE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHealthComponent();

	/* PROPERTIES */

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnDead"))
	FDeadDelegate DeadDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnHealthChanged"))
	FHealthChangedDelegate HealthChangedDelegate;

	/* FUNCTIONS */

	UFUNCTION(BlueprintPure)
	const float GetHealth() { return Health; };

	UFUNCTION(BlueprintCallable)
	void SetHealth(float Value);

	UFUNCTION(BlueprintPure)
	const float GetMaxHealth() { return MaxHealth; };

	UFUNCTION(BlueprintPure)
	const bool GetDead() { return bDead; };

	UFUNCTION(BlueprintCallable)
	void SetDead(bool bValue);

protected:

	/* FUNCTIONS */

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnAnyDamage"))
	void ReceiveOnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnRep_SetHealth();

	UFUNCTION()
	virtual void OnRep_SetDead();

private:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_SetHealth)
	float Health;

	UPROPERTY(Replicated)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_SetDead)
	bool bDead;
};
