
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Inventory/LyraInventoryItemFragment.h"

ULyraInventoryItemDefinition::ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
{
}

const ULyraInventoryItemFragment* ULyraInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
	if(FragmentClass != nullptr)
	{
		for (const ULyraInventoryItemFragment* Fragment : Fragments)
		{
			if(Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

void ULyraInventoryItemDefinition::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTags.CombinedTags);
}

int32 ULyraInventoryItemDefinition::GetMaxStackSize() const
{
	return MaxStackSize;
}
