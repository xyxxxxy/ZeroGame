
#include "LyraInventoryLibrary.h"
#include "AbilitySystemComponent.h"
#include "Inventory/LyraInventoryComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "LyraInventory/LyraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryLibrary)

bool ULyraInventoryLibrary::ASCHasAttributeSet(UAbilitySystemComponent* ASC, TSubclassOf<UAttributeSet> AttributeSetClass)
{
	for (const UAttributeSet* AttributeSet : ASC->GetSpawnedAttributes())
	{
		if (AttributeSet->GetClass() == AttributeSetClass)
		{
			return true;
		}
	}

	return false;
}

bool ULyraInventoryLibrary::ASCAddInstancedAttributeSet(UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet)
{
	if (ASC->GetOwnerRole() != ROLE_Authority)
	{
		return false;
	}
	if (!IsValid(AttributeSet))
	{
		return false;
	}
	//We don't want to double up attributes.  If we already have this attribute set added we need to fix that
	ensure(!ASC->GetSpawnedAttributes().Contains(AttributeSet));
	
	ASC->AddSpawnedAttribute(AttributeSet);

	//ASC->bIsNetDirty = true;

	return false;
}

bool ULyraInventoryLibrary::FindFragment_UI(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_UI& FragmentStruct)
{
	if(!IsValid(ItemInstance))return false;

	FLyraInventoryTableRowFragmentsHandle FragmentsHandle = ItemInstance->GetItemStructHandle();
	
	if(!FragmentsHandle.Data.Get()->ItemStructs.IsEmpty())
	{
		return  FragmentsHandle.FindFragment_UI(FragmentStruct);
	}
	return false;
}

bool ULyraInventoryLibrary::FindFragment_World(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_World& FragmentStruct)
{
	if(!IsValid(ItemInstance))return false;

	FLyraInventoryTableRowFragmentsHandle FragmentsHandle = ItemInstance->GetItemStructHandle();
	
	if(!FragmentsHandle.Data.Get()->ItemStructs.IsEmpty())
	{
		return  FragmentsHandle.FindFragment_World(FragmentStruct);
	}
	return false;
}

bool ULyraInventoryLibrary::FindFragment_AbilitySet(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_AbilitySet& FragmentStruct)
{
	if(!IsValid(ItemInstance))return false;

	FLyraInventoryTableRowFragmentsHandle FragmentsHandle = ItemInstance->GetItemStructHandle();
	
	if(!FragmentsHandle.Data.Get()->ItemStructs.IsEmpty())
	{
		return  FragmentsHandle.FindFragment_AbilitySet(FragmentStruct);
	}
	return false;
}

FLyraInventoryItemSlot& ULyraInventoryLibrary::GetItemSlot(const FLyraInventoryItemSlotHandle& Handle)
{
	return Handle.ParentInventory->GetItemSlot(Handle);
}






