
#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "Equipment/LyraEquipmentTypes.h"
#include "Styling/SlateBrush.h"
#include "Inventory/LyraItemEnum.h"
#include "LyraInventoryTableRow.generated.h"

class ULyraInventoryItemInstance;
class UTexture2D;
class UStaticMesh;

// For Struct
USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryTableRowFragment
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragment(){}
	
};

USTRUCT(meta = (DisplayName = "Fragment_UI", ScriptName = "Fragment_UI"))
struct LYRAINVENTORY_API FLyraInventoryTableRowFragment_UI : public FLyraInventoryTableRowFragment
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragment_UI(){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true), Category = "Inventory")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FSlateBrush Brush;
};

USTRUCT(meta = (DisplayName = "Fragment_World", ScriptName = "Fragment_World"))
struct LYRAINVENTORY_API FLyraInventoryTableRowFragment_World : public FLyraInventoryTableRowFragment
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragment_World(){}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UStaticMesh> Mesh;
};

USTRUCT(meta = (DisplayName = "Fragment_AbilitySet", ScriptName = "Fragment_AbilitySet"))
struct LYRAINVENTORY_API FLyraInventoryTableRowFragment_AbilitySet : public FLyraInventoryTableRowFragment
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragment_AbilitySet(){}
	
	//bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FLyraAbilitySet_GrantedHandles* OutHandleStorage, ULyraInventoryItemInstance* SourceObject) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<ULyraAbilitySet> AbilitySet;

};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryTableRowFragmentsArray
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragmentsArray(){}
	FLyraInventoryTableRowFragmentsArray(const FLyraInventoryTableRowFragmentsArray& Other)
	: ItemTags(Other.ItemTags), ItemStructs(Other.ItemStructs)
	{
	}
	FLyraInventoryTableRowFragmentsArray(const FGameplayTagContainer& InTags, const TArray<FInstancedStruct>& InItemStructs)
	: ItemTags(InTags), ItemStructs(InItemStructs)
	{
	}

	// GameplayTags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FGameplayTagContainer ItemTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (BaseStruct = "/Script/LyraInventory.LyraInventoryTableRowFragment", ExcludeBaseStruct), Category = "Inventory")
	TArray<FInstancedStruct> ItemStructs;
	
	bool IsValid() const
	{
		if(ItemStructs.IsEmpty())
		{
			return false;
		}
		return true;
	}

	bool operator==(const FLyraInventoryTableRowFragmentsArray& Other) const
	{
		const bool bTagMatch = ItemTags == Other.ItemTags;
		const bool bFragmentMatch = ItemStructs == Other.ItemStructs;
		return  bTagMatch && bFragmentMatch;
	}
	
	bool operator!=(const FLyraInventoryTableRowFragmentsArray& Other) const
	{
		return !(FLyraInventoryTableRowFragmentsArray::operator==(Other));
	}
};


USTRUCT(BlueprintType)
struct  LYRAINVENTORY_API FLyraItemInfoRowDefine : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FName ItemName = FName();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FLyraInventoryTableRowFragmentsArray CustomItemInfo;
	
};

/**
* Handle of comp to get item data.
**/
USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryTableRowFragmentsHandle
{
	GENERATED_BODY()
public:
	FLyraInventoryTableRowFragmentsHandle() : Data(nullptr)
	{
	}
	FLyraInventoryTableRowFragmentsHandle(FLyraInventoryTableRowFragmentsArray* FragmentsArray) : Data(FragmentsArray){}
	FLyraInventoryTableRowFragmentsHandle(const FLyraInventoryTableRowFragmentsHandle& Other)
	: Data(Other.Data)
	{
	}
	
	bool IsValid() const
	{
		return Data.IsValid();
	}
	
	bool FindFragment_UI(FLyraInventoryTableRowFragment_UI& OutFragment) const;
	bool FindFragment_World(FLyraInventoryTableRowFragment_World& OutFragment) const;
	bool FindFragment_AbilitySet(FLyraInventoryTableRowFragment_AbilitySet& OutFragment) const;
	
	TSharedPtr<FLyraInventoryTableRowFragmentsArray> Data;
};