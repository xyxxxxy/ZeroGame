
#include "Inventory/LyraInventoryItemInstance.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryItemInstance)

ULyraInventoryItemInstance::ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

FLyraInventoryTableRowFragmentsHandle ULyraInventoryItemInstance::GetItemStructHandle() const
{
	return ItemStructsHandle;
}

void ULyraInventoryItemInstance::SetItemStructsHandle(const FLyraInventoryTableRowFragmentsHandle& InHandle)
{
	ItemStructsHandle = InHandle;
}

void ULyraInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if(!ItemStructsHandle.Data.Get()->ItemStructs.IsEmpty())
	{
		TagContainer.AppendTags(ItemStructsHandle.Data.Get()->ItemTags);
	}
}

bool ULyraInventoryItemInstance::CanStackWith(ULyraInventoryItemInstance* Other)
{
	if(!IsValid(Other))return false;

	if(!ItemStructsHandle.IsValid() || !Other->GetItemStructHandle().IsValid())
	{
		return false;
	}
	FLyraInventoryTableRowFragment_UI Fragment_UI;
	if(ItemStructsHandle.FindFragment_UI(Fragment_UI))
	{
		bool bCanStack = Fragment_UI.MaxStackSize > 1;
		return bCanStack && *ItemStructsHandle.Data.Get() == *Other->GetItemStructHandle().Data.Get();
	}
	return false;
}

bool ULyraInventoryItemInstance::MergeItemStacks(ULyraInventoryItemInstance* Other)
{
	if(!IsValid(Other))return false;
	if(!CanStackWith(Other))return false;

	FLyraInventoryTableRowFragment_UI Fragment_UI;
	if(ItemStructsHandle.FindFragment_UI(Fragment_UI))
	{
		const int32 MaxStackSize = Fragment_UI.MaxStackSize;
		if(StackSize + Other->StackSize > MaxStackSize)
		{
			int32 Diff = StackSize + Other->StackSize - MaxStackSize;
			StackSize = MaxStackSize;
			Other->StackSize = Diff;
			return true;
		}
		else
		{
			StackSize = StackSize + Other->StackSize;
			return true;
		}
	}
	return false;
	
}










// const ULyraInventoryItemFragment* ULyraInventoryItemInstanceBase::FindFragmentByClass(
// 	TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
// {
// 	if(FragmentClass != nullptr)
// 	{
// 		if(ItemDefinition != nullptr)
// 		{
// 			return ItemDefinition->FindFragmentByClass(FragmentClass);
// 		}
// 		for(const ULyraInventoryItemFragment* Fragment : DynamicFragments)
// 		{
// 			if(Fragment && Fragment->IsA(FragmentClass))
// 			{
// 				return Fragment;
// 			}
// 		}
// 	}
// 	return nullptr;
// }
