#pragma once

#include "CoreMinimal.h"
#include "Items/ItemData.h"
#include "WeaponData.generated.h"


UCLASS()
class TESTTASK_4TALE_API UWeaponData : public UItemData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
	float UseRate;

	UPROPERTY(EditAnywhere)
	bool bAutoFire;

	UPROPERTY(EditAnywhere)
	float ReloadRate;

	UPROPERTY(EditAnywhere)
	bool bInfinityAmmo;

	UWeaponData()
	{
		Damage = 1.0f;
		Ammo = 0;
		UseRate = 0.1f;
		bAutoFire = false;
		ReloadRate = 0.5f;
		bInfinityAmmo = false;
	}
};
