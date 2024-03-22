#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "BaseItem.generated.h"


UCLASS(Blueprintable)
class TESTTASK_4TALE_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseItem();

	/* PROPERTIES */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemData* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachSocketName;

	/* FUNCTIONS */

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	virtual void HideAndAttach();

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	virtual void ShowAndAttach(USceneComponent* ParentComp);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem() {};

	UFUNCTION(BlueprintCallable)
	virtual void StopUseItem() {};

	UFUNCTION(BlueprintCallable)
	virtual void InitParams() {};

protected:

	/* FUNCTIONS */

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};