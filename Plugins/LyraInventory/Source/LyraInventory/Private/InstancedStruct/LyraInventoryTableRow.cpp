
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "AbilitySystemComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "LyraInventory/LyraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryTableRow)

bool FLyraInventoryTableRowFragment_AbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
                                                                      FLyraAbilitySet_GrantHandles* OutHandleStorage, ULyraInventoryItemInstance* SourceObject) const
{
	if(ASC == nullptr)return false;
	
	if(OutHandleStorage == nullptr)
	{
		UE_LOG(LogLyraInventorySystem, Error, TEXT("Out Grant Handle is not valid!"));
		return false;
	}

	for (const auto& AbilityToGrant : EquipmentAbilitySet.GrantAbilities)
	{
		if(!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogLyraInventorySystem, Warning, TEXT("Granted GA on equipment Struct FLyraInventoryTableRowFragment_AbilitySet is not valid."));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;

		FGameplayAbilitySpecHandle AbilitySpecHandle;
		// TODO: passive or active GA
		AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		OutHandleStorage->AddAbilitySpecHandle(AbilitySpecHandle);
	}

	for (const auto& EffectToGrant : EquipmentAbilitySet.GrantEffects)
	{
		if(!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogLyraInventorySystem, Warning, TEXT("Granted GE on equipment Struct FLyraInventoryTableRowFragment_AbilitySet is not valid."));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect.GetDefaultObject();
		
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(
			GameplayEffect
			,EffectToGrant.EffectLevel
			,ASC->MakeEffectContext()
			);
		OutHandleStorage->AddEffectSpecHandle(GameplayEffectHandle);
	}
	return true;
}



bool FLyraInventoryTableRowFragmentsHandle::FindFragment_UI(FLyraInventoryTableRowFragment_UI& OutFragment) const
{
	for(const FInstancedStruct& InstancedStruct : Data.Get()->ItemStructs)
	{
		if(const auto Fragment = InstancedStruct.GetPtr<FLyraInventoryTableRowFragment_UI>())
		{
			OutFragment = *Fragment;
			return true;
		}
	}
	return false;
}

bool FLyraInventoryTableRowFragmentsHandle::FindFragment_World(FLyraInventoryTableRowFragment_World& OutFragment) const
{
	for(const FInstancedStruct& InstancedStruct : Data.Get()->ItemStructs)
	{
		if(const auto Fragment = InstancedStruct.GetPtr<FLyraInventoryTableRowFragment_World>())
		{
			OutFragment = *Fragment;
			return true;
		}
	}
	return false;
}

bool FLyraInventoryTableRowFragmentsHandle::FindFragment_AbilitySet(FLyraInventoryTableRowFragment_AbilitySet& OutFragment) const
{
	for(const FInstancedStruct& InstancedStruct : Data.Get()->ItemStructs)
	{
		if(const auto Fragment = InstancedStruct.GetPtr<FLyraInventoryTableRowFragment_AbilitySet>())
		{
			OutFragment = *Fragment;
			return true;
		}
	}
	return false;
}
