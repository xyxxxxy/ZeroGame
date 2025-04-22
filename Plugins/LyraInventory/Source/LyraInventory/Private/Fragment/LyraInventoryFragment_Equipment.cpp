
#include "Fragment/LyraInventoryFragment_Equipment.h"
#include "AbilitySystemComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "InventoryLogChannels.h"

// bool ULyraInventoryFragment_Equipment::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
//                                                            FLyraAbilitySet_GrantedHandles* OutHandleStorage, ULyraInventoryItemInstance* SourceObject) const
// {
// 	if(ASC == nullptr)return false;
	
// 	if(OutHandleStorage == nullptr)
// 	{
// 		UE_LOG(LogLyraInventorySystem,Error,TEXT("Out Grant Handle is not valid!"));
// 		return false;
// 	}

// 	for (const auto& AbilityToGrant : EquipmentAbilitySet.GrantAbilities)
// 	{
// 		if(!IsValid(AbilityToGrant.Ability))
// 		{
// 			UE_LOG(LogLyraInventorySystem,Error,TEXT("Granted GA on equipment fragment [%s] is not valid."),*GetNameSafe(this));
// 			continue;
// 		}

// 		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
// 		FGameplayAbilitySpec AbilitySpec(AbilityCDO,AbilityToGrant.AbilityLevel);
// 		AbilitySpec.SourceObject = SourceObject;

// 		FGameplayAbilitySpecHandle AbilitySpecHandle;
// 		// TODO: passive or active GA
// 		AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

// 		OutHandleStorage->AddAbilitySpecHandle(AbilitySpecHandle);
// 	}

// 	for (const auto& EffectToGrant : EquipmentAbilitySet.GrantEffects)
// 	{
// 		if(!IsValid(EffectToGrant.PassiveGameplayEffect))
// 		{
// 			UE_LOG(LogLyraInventorySystem,Error,TEXT("Granted GE on equipment fragment [%s] is not valid."),*GetNameSafe(this));
// 			continue;
// 		}

// 		const UGameplayEffect* GameplayEffect = EffectToGrant.PassiveGameplayEffect.GetDefaultObject();
		
// 		// FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
// 		// EffectContextHandle.AddSourceObject(SourceObject);
// 		// const FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectToGrant.GameplayEffect,EffectToGrant.EffectLevel,EffectContextHandle);
// 		// ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		
// 		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(
// 			GameplayEffect
// 			,EffectToGrant.EffectLevel
// 			,ASC->MakeEffectContext()
// 			);
// 		OutHandleStorage->AddGameplayEffectHandle(GameplayEffectHandle);
// 	}
// 	return true;
// }
