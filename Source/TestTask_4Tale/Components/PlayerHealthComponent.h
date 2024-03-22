#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "PlayerHealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTTASK_4TALE_API UPlayerHealthComponent : public UHealthComponent
{
	GENERATED_BODY()
	
public:

	/* FUNCTIONS */

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ResetStats();
};
