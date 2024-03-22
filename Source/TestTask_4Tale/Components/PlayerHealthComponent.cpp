#include "Components/PlayerHealthComponent.h"

// For player respawn logic
void UPlayerHealthComponent::ResetStats_Implementation()
{
	if (GetOwner()->HasAuthority())
	{
		SetHealth(GetMaxHealth());
		SetDead(false);
	}
}