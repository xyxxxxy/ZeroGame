
#include "Inventory/LyraInventoryItemTypes.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryItemTypes)

bool FLyraInventoryItemSlotFilter::AcceptsItem(ULyraInventoryItemInstance* ItemInstance) const
{
	if(!IsValid(ItemInstance))
	{
		return false;
	}

	// TODO : Stack = 1

	FGameplayTagContainer StackTags;
	ItemInstance->GetOwnedGameplayTags(StackTags);
	if(!FilterQuery.IsEmpty())
	{
		if(!FilterQuery.Matches(StackTags))
		{
			return false;
		}
	}
	return true;
}

bool FLyraInventoryItemSlotFilter::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayTagQuery::StaticStruct()->GetCppStructOps()->NetSerialize(Ar, Map, bOutSuccess, &FilterQuery);
	Ar << bForceSingleStack;

	bOutSuccess = true;
	return true;	
}

/////////////////////////////////////////

bool FLyraInventoryItemFilterHandle::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;

	bOutSuccess = true;

	if (Ar.IsSaving())
	{
		if (Data.IsValid())
		{
			RepBits |= 1 << 0;
		}
	}

	Ar.SerializeBits(&RepBits, 1);

	if (RepBits & (1 << 0))
	{
		if (Ar.IsLoading())
		{
			Data = MakeShareable(new FLyraInventoryItemSlotFilter());
		}

		FLyraInventoryItemSlotFilter::StaticStruct()->GetCppStructOps()->NetSerialize(Ar, Map, bOutSuccess, Data.Get());
	}

	return true;
}

////////////////////////////////////////
bool FLyraInventoryQuery::MatchesSlot(const FLyraInventoryItemSlot& ItemSlot) const
{
	bool bMatches = true;

	if(!SlotTypeQuery.IsEmpty())
	{
		bMatches &= SlotTypeQuery.Matches(ItemSlot.SlotTags);
	}

	if(!ItemTypeQuery.IsEmpty())
	{
		if(!::IsValid(ItemSlot.ItemInstance))
		{
			return false;
		}
		FGameplayTagContainer StackContainer;
		ItemSlot.ItemInstance->GetOwnedGameplayTags(StackContainer);
		

		bMatches &= ItemTypeQuery.Matches(StackContainer);
	}
	return bMatches;
}

FLyraInventoryQuery FLyraInventoryQuery::QuerySlotMatchingTag(FGameplayTag Tag)
{
	FLyraInventoryQuery Query;
	Query.SlotTypeQuery = FGameplayTagQuery::MakeQuery_MatchTag(Tag);
	return Query;
}

FLyraInventoryQuery FLyraInventoryQuery::QueryForMatchingItemTag(FGameplayTag Tag)
{
	FLyraInventoryQuery Query;
	Query.ItemTypeQuery = FGameplayTagQuery::MakeQuery_MatchTag(Tag);
	return Query;
}

FLyraInventoryQuery FLyraInventoryQuery::QueryForSlot(const FGameplayTagQuery& SlotQuery)
{
	FLyraInventoryQuery Query;
	Query.SlotTypeQuery = SlotQuery;
	return Query;
}

FLyraInventoryQuery FLyraInventoryQuery::QueryForItemType(const FGameplayTagQuery& ItemQuery)
{
	FLyraInventoryQuery Query;
	Query.ItemTypeQuery = ItemQuery;
	return Query;
}

bool FLyraInventoryQuery::IsValid() const
{
	return !ItemTypeQuery.IsEmpty() || !SlotTypeQuery.IsEmpty();
}
