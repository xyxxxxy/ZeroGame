
#include "Equipment/LyraEquipmentTypes.h"
#include "AbilitySystemComponent.h"

void FLyraAbilitySet_GrantHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if(Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FLyraAbilitySet_GrantHandles::AddEffectSpecHandle(const FActiveGameplayEffectHandle& Handle)
{
	if(Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FLyraAbilitySet_GrantHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if(ASC)
	{
		for (const auto& Handle : AbilitySpecHandles)
		{
			ASC->ClearAbility(Handle);
		}
		for (const auto& Handle : GameplayEffectHandles)
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}
	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}
