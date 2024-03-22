#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerWeaponInterface.generated.h"


UINTERFACE(MinimalAPI)
class UPlayerWeaponInterface : public UInterface
{
	GENERATED_BODY()
};


class TESTTASK_4TALE_API IPlayerWeaponInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(Blueprintable, BlueprintImplementableEvent)
	FVector GetFireDir();
};
