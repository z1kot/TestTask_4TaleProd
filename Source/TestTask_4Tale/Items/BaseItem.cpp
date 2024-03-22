#include "BaseItem.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseItem::ABaseItem()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	AttachSocketName = FName(TEXT("ItemSocket"));
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InitParams();
	}
}

void ABaseItem::HideAndAttach_Implementation()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	if (GetOwner())
	{
		AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ABaseItem::ShowAndAttach_Implementation(USceneComponent* ParentComp)
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(false);

	if (ParentComp)
	{
		AttachToComponent(ParentComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
	}
}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}