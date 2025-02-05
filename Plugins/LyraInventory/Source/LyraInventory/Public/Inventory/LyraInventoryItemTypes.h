
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LyraItemEnum.h"
#include "LyraInventoryItemTypes.generated.h"

class ULyraInventoryComponent;
class ULyraInventoryItemInstance;
class ULyraInventoryProcessor_Bag;

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemHandleEntry 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 SlotId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FGameplayTagContainer SlotTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	ELyraItemCategory ItemCategory = ELyraItemCategory::None;

};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemSlotFilter
{
	GENERATED_BODY()
public:
	FLyraInventoryItemSlotFilter()
	{
		bForceSingleStack = false;
	}
	FLyraInventoryItemSlotFilter(const FLyraInventoryItemSlotFilter& Other)
	: FilterQuery(Other.FilterQuery), bForceSingleStack(Other.bForceSingleStack)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	FGameplayTagQuery FilterQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bForceSingleStack;

	bool AcceptsItem(ULyraInventoryItemInstance* ItemInstance) const;
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FLyraInventoryItemSlotFilter> : public TStructOpsTypeTraitsBase2<FLyraInventoryItemSlotFilter>
{
	enum
	{
		WithNetSerializer = true,
	};
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemFilterHandle
{
	GENERATED_BODY()
public:
	FLyraInventoryItemFilterHandle():Data(nullptr)
	{
	}
	
	FLyraInventoryItemFilterHandle(FLyraInventoryItemSlotFilter* Filter)
	:Data(Filter)
	{
	}

	bool IsValid() const
	{
		return Data.IsValid();
	}

	bool AcceptsItem(ULyraInventoryItemInstance* ItemInstance) const
	{
		if(!IsValid())
		{
			return false;
		}
		return Data->AcceptsItem(ItemInstance);
	}
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	TSharedPtr<FLyraInventoryItemSlotFilter> Data;
	
};

template<>
struct TStructOpsTypeTraits<FLyraInventoryItemFilterHandle> : public TStructOpsTypeTraitsBase2<FLyraInventoryItemFilterHandle>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};


USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemSlot
{
	GENERATED_BODY()
public:
	FLyraInventoryItemSlot()
	:ItemInstance(nullptr), SlotId(0), SlotTags(),ItemCategory(ELyraItemCategory::Prop)
	{
	}
	
	FLyraInventoryItemSlot(const FLyraInventoryItemSlot& InSlot)
	:ItemInstance(InSlot.ItemInstance), SlotId(InSlot.SlotId), SlotTags(InSlot.SlotTags), ItemCategory(InSlot.ItemCategory), ItemSlotFilter(InSlot.ItemSlotFilter)
	{
	}
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Inventory")
	TObjectPtr<ULyraInventoryItemInstance> ItemInstance;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	int32 SlotId;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	FGameplayTagContainer SlotTags;
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	ELyraItemCategory ItemCategory;

	UPROPERTY()
	FLyraInventoryItemFilterHandle ItemSlotFilter;
	
	bool operator==(const FLyraInventoryItemSlot& Other) const{return this->SlotId == Other.SlotId && this->ItemInstance == Other.ItemInstance;}
	bool operator!=(const FLyraInventoryItemSlot& Other) const{return !(FLyraInventoryItemSlot::operator==(Other));}

protected:
	TObjectPtr<ULyraInventoryComponent> Owner;
	//friend class ULyraInventoryComponent;
	friend class ULyraInventoryProcessor_Bag;
	
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FAuraInventoryItemArray
{
	GENERATED_BODY()
public:
	FAuraInventoryItemArray()
	: FAuraInventoryItemArray(nullptr)
	{}
	FAuraInventoryItemArray(ULyraInventoryComponent* InOwner)
		: Owner(InOwner)
	{}
	
	UPROPERTY()
	TArray<FLyraInventoryItemSlot> Slots;

	UPROPERTY()
	TObjectPtr<ULyraInventoryComponent> Owner;
};

// Key of slot
USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemSlotHandle
{
	GENERATED_BODY()
public:
	FLyraInventoryItemSlotHandle()
	: SlotId(-1), SlotTags(), ItemCategory(ELyraItemCategory::Prop), ParentInventory(nullptr)
	{
	}
	
	FLyraInventoryItemSlotHandle(const FLyraInventoryItemSlotHandle& InHandle)
	: SlotId(InHandle.SlotId), SlotTags(InHandle.SlotTags), ItemCategory(InHandle.ItemCategory), ParentInventory(InHandle.ParentInventory)
	{
	}
	
	FLyraInventoryItemSlotHandle(const FLyraInventoryItemSlot& FromSlot, ULyraInventoryComponent* InParentInventory)
	: SlotId(FromSlot.SlotId), SlotTags(FromSlot.SlotTags), ItemCategory(FromSlot.ItemCategory), ParentInventory(InParentInventory)
	{
	}

	FLyraInventoryItemSlotHandle(int32 InSlotId, ULyraInventoryComponent* InParentInventory)
	: SlotId(InSlotId), SlotTags(), ItemCategory(ELyraItemCategory::Prop), ParentInventory(InParentInventory)
	{
	}
	FLyraInventoryItemSlotHandle(FGameplayTag Tag, ULyraInventoryComponent* InParentInventory)
	: SlotId(0), SlotTags(Tag.GetSingleTagContainer()), ItemCategory(ELyraItemCategory::Prop), ParentInventory(InParentInventory)
	{
	}
	

	bool GetIsValid() const {return SlotId > 0;}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	int32 SlotId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	FGameplayTagContainer SlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
	ELyraItemCategory ItemCategory;
	
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<ULyraInventoryComponent> ParentInventory;

	bool operator==(const FLyraInventoryItemSlotHandle& Other) const
	{
		const bool bIdMatch = SlotId == Other.SlotId;
		const bool bInventoryMatch = ParentInventory == Other.ParentInventory;
		const bool bTagsMatch = SlotTags.HasAllExact(Other.SlotTags);
		const bool bIsCategoryMatch = ItemCategory == Other.ItemCategory;
		return bIdMatch && bInventoryMatch && bTagsMatch && bIsCategoryMatch;
	}

	bool operator!=(const FLyraInventoryItemSlotHandle& Other) const
	{
		return !(FLyraInventoryItemSlotHandle::operator==(Other));
	}

	bool operator==(const FLyraInventoryItemSlot& Other) const
	{
		const bool bIdMatch = SlotId == Other.SlotId;
		const bool bTagsMatch = SlotTags.HasAllExact(Other.SlotTags);
		const bool bIsCategoryMatch = ItemCategory == Other.ItemCategory;
		return bIdMatch && bTagsMatch && bIsCategoryMatch;
	}

	bool operator!=(const FLyraInventoryItemSlot& Slot) const
	{
		return !(FLyraInventoryItemSlotHandle::operator==(Slot));
	}
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryQuery
{
	GENERATED_BODY()
public:
	FLyraInventoryQuery()
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	ELyraItemCategory Category = ELyraItemCategory::Prop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	FGameplayTagQuery ItemTypeQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	FGameplayTagQuery SlotTypeQuery;

	/**
	 *  If empty,return true. Query both item tags and slot tags
	 */
	bool MatchesSlot(const FLyraInventoryItemSlot& ItemSlot) const;
	
	static FLyraInventoryQuery QuerySlotMatchingTag(FGameplayTag Tag);
	static FLyraInventoryQuery QueryForMatchingItemTag(FGameplayTag Tag);
	static FLyraInventoryQuery QueryForSlot(const FGameplayTagQuery& SlotQuery);
	static FLyraInventoryQuery QueryForItemType(const FGameplayTagQuery& ItemQuery);

	bool IsValid() const;
};

LYRAINVENTORY_API inline bool IsValid(const FLyraInventoryItemFilterHandle& FilterHandle)
{
	return FilterHandle.IsValid();
}

LYRAINVENTORY_API inline bool IsValid(const FLyraInventoryItemSlotHandle& SlotHandle)
{
	return SlotHandle.GetIsValid();
}