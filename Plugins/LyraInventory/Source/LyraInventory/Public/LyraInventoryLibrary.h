
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Inventory/LyraInventoryItemTypes.h"
#include "LyraInventoryLibrary.generated.h"

class ULyraInventoryComponent_Bag;
class ULyraInventoryItemInstance;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class LYRAINVENTORY_API ULyraInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory | Attribute")
	static bool ASCHasAttributeSet(UAbilitySystemComponent* ASC,TSubclassOf<UAttributeSet> AttributeSetClass);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Attribute")
	static bool ASCAddInstancedAttributeSet(UAbilitySystemComponent* ASC,UAttributeSet* AttributeSet);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory | Struct")
	static bool FindFragment_UI(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_UI& FragmentStruct);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Struct")
	static bool FindFragment_World(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_World& FragmentStruct);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Struct")
	static bool FindFragment_AbilitySet(ULyraInventoryItemInstance* ItemInstance, FLyraInventoryTableRowFragment_AbilitySet& FragmentStruct);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FLyraInventoryItemSlot& GetItemSlot(const FLyraInventoryItemSlotHandle& Handle);
	
};
